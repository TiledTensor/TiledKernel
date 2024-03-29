#include "graph/tilednode.hpp"
#include "graph/tilededge.hpp"
#include "generator.hpp"
#include "error_handler.hpp"
#include <fmt/core.h>
#include <fmtlog.h>
#include <sstream>
#include <iostream>

namespace tiledkernel {

    TiledGenerator::TiledGenerator() { ctx = std::make_shared<TiledContext>(); }

    std::string TiledGenerator::emit(Platform platform,
                                     TiledGraph::Pointer graph) {
        switch (platform) {
            case Platform::Cute:
                return emit_cute(graph);
            default:
                return "";
        }
    }

    std::string TiledGenerator::emit_cute(TiledGraph::Pointer graph) {
        std::string kernel = "";
        switch (graph->getMemLevel()) {
            case MemoryLevel::Global:
                kernel += emit_global_cute(graph);
            case MemoryLevel::Shared:
                kernel += emit_shared_cute(graph);
            case MemoryLevel::RF:
                kernel += emit_rf_cute(graph);
        }
        return kernel;
    }

    std::string TiledGenerator::emit_rf_cute(TiledGraph::Pointer graph) {
        std::string kernel;
        auto sorted_nodes = graph->topoSort();

        // Find compute nodes
        std::vector<TiledNode::Pointer> compute_nodes;
        for (auto node : sorted_nodes) {
            if (node->getType() == NodeType::Operator) {
                compute_nodes.push_back(node);
            }
        }

        // Emit compute kernel
        for (auto node : compute_nodes) {
            // auto in_edges = node->getInEdges();
            // auto out_edges = node->getOutEdges();

            switch (node->getOpType()) {
                case OpType::Gemm:
                    kernel += emit_rf_cute_gemm(node);
                    break;
                default:
                    TODO("Operator not supported yet.");
            }
        }
        return kernel;
    }

    std::string TiledGenerator::emit_shared_cute(TiledGraph::Pointer graph) {
        std::string kernel;
        auto sorted_nodes = graph->topoSort();

        std::vector<TiledNode::Pointer> compute_nodes;
        std::vector<TiledNode::Pointer> rf_nodes;

        for (auto node : sorted_nodes) {
            if (node->getMemLevel() == MemoryLevel::RF) {
                rf_nodes.push_back(node);
            }
        }

        kernel += "__syncthreads();\n";
        for (auto node : rf_nodes) {
            auto graph = std::get<TiledGraph::Pointer>(node->getData());
            auto sorted_nodes = graph->topoSort();
#ifdef DEBUG
            fmt::println("[DEBUG] ID\tName");
            for (auto node : sorted_nodes) {
                fmt::println("[DEBUG] {}\t{}", node->id.value(), node->name);
            }
#endif
            kernel += emit_rf_cute(graph);
        }
        kernel += "__syncthreads();\n";

        return kernel;
    }

    std::string TiledGenerator::emit_global_cute(TiledGraph::Pointer graph) {
        return "";
    }

    std::string TiledGenerator::emit_rf_cute_gemm(TiledNode::Pointer node) {
        auto predecessors = node->getPredecessors();
        auto successors = node->getSuccessors();

        ASSERT(predecessors.size() == 2,
               "Gemm node should have 2 predecessors.");
        ASSERT(successors.size() == 1, "Gemm node should have 1 successor.");

        auto rA = predecessors[0];
        auto rB = predecessors[1];
        auto acc = successors[0];

        auto gemm_in_edges = node->getInEdges();
        auto gemm_out_edges = node->getOutEdges();

        ASSERT(gemm_in_edges.size() == 2, "Gemm node should have 2 in edges.");
        ASSERT(gemm_out_edges.size() == 1, "Gemm node should have 1 out edge.");

        // TODO: Add `access_map` to `TiledNode` to store the access pattern.
        std::string kernel;

        // TODO: Generate  `for` loop.
        // TODO: Check if the access map is valid.
        auto access_map_rA = gemm_in_edges[0]->getAccessMapI();
        auto access_map_rB = gemm_in_edges[1]->getAccessMapI();
        auto access_map_acc = gemm_out_edges[0]->getAccessMapI();

        ASSERT(access_map_rA->getLoops() == access_map_rB->getLoops(),
               "Access map should have the same number of loops.");
        ASSERT(access_map_rA->getLoops() == access_map_acc->getLoops(),
               "Access map should have the same number of loops.");

        for (size_t loop = 0; loop < access_map_rA->getLoops(); loop++) {
            if (access_map_rA->hasPinIterVar(loop)) {
                ASSERT(access_map_rB->hasPinIterVar(loop),
                       "Access map should have the same pin iter var.");
                ASSERT(access_map_acc->hasPinIterVar(loop),
                       "Access map should have the same pin iter var.");

                // Check if the pin iter var is the same.
                ASSERT(access_map_rA->pin_iter_vars[loop].value()->id ==
                           access_map_rB->pin_iter_vars[loop].value()->id,
                       "Pin iter var should be the same.");
                ASSERT(access_map_rA->pin_iter_vars[loop].value()->id ==
                           access_map_acc->pin_iter_vars[loop].value()->id,
                       "Pin iter var should be the same.");
            } else {
                auto iter_var = std::make_shared<IterVar>(
                    type::DataType::Int32,
                    access_map_rA->iteration_domain[loop].first,
                    access_map_rA->iteration_domain[loop].second, 1);

                ctx->pushVar(iter_var);
                access_map_rA->setPinIterVar(loop, iter_var);
                access_map_rB->setPinIterVar(loop, iter_var);
                access_map_acc->setPinIterVar(loop, iter_var);
            }
        }

        std::string rA_access =
            rA->getBufferName().value() + generate_access_map(access_map_rA);
        std::string rB_access =
            rB->getBufferName().value() + generate_access_map(access_map_rB);
        std::string acc_access =
            acc->getBufferName().value() + generate_access_map(access_map_acc);

        // TODO: Use macro kernel instead of hardcoding the kernel.
        auto kernel_body = fmt::format("gemm({}, {}, {});\n", rA_access,
                                       rB_access, acc_access);

        for (size_t loop = 0; loop < access_map_rA->getLoops(); loop++) {
            kernel_body = generate_loop(
                access_map_rA->pin_iter_vars[loop].value(), kernel_body);
        }

        kernel += kernel_body;

        return kernel;
    }

    std::string TiledGenerator::generate_loop(std::shared_ptr<IterVar> iter_var,
                                              std::string body) {
        std::string kernel;
        uint64_t indient = 0;
        kernel += fmt::format("for (int {} = {}; {} < {}; {} += {}) {{\n",
                              iter_var->name, iter_var->start, iter_var->name,
                              iter_var->end, iter_var->name, iter_var->step);
        indient += 4;
        std::istringstream iss(body);
        std::string line;
        while (std::getline(iss, line)) {
            kernel += fmt::format("{}{}\n", std::string(indient, ' '), line);
        }
        indient -= 4;
        kernel += fmt::format("{}}}\n", std::string(indient, ' '));
        return kernel;
    }

    std::string TiledGenerator::generate_access_map(
        AccessMap::Pointer access_map) {
        std::string kernel;
        for (size_t access_loop = 0; access_loop < access_map->getAccessDims();
             access_loop++) {
            auto access_dim = access_map->access_pattern[access_loop];
            for (size_t loop = 0; loop < access_map->getLoops(); loop++) {
                if (access_dim[loop] == 0) {
                    return "";
                } else if (access_dim[loop] == 1) {
                    kernel += fmt::format(
                        "[{}]", access_map->pin_iter_vars[loop].value()->name);
                } else {
                    kernel += fmt::format(
                        "[{} * {}]",
                        access_map->pin_iter_vars[loop].value()->name,
                        access_dim[loop]);
                }
            }
        }
        return kernel;
    }

}  // namespace tiledkernel