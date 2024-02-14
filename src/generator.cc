#include "graph/tilednode.hpp"
#include "graph/tilededge.hpp"
#include "generator.hpp"
#include "error_handler.hpp"
#include <fmt/core.h>

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
                break;
            case MemoryLevel::Shared:
                break;
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

        uint64_t indient = 0;
        for (auto loop = 0; loop < access_map_rA->getLoops(); loop++) {
            if (access_map_rA->hasPinIterVar(loop)) {
                ASSERT(access_map_rB->hasPinIterVar(loop),
                       "Access map should have the same pin iter var.");
                ASSERT(access_map_acc->hasPinIterVar(loop),
                       "Access map should have the same pin iter var.");
            } else {
                auto iter_var = std::make_shared<IterVar>(
                    type::DataType::Int32,
                    access_map_rA->iteration_domain[loop].first,
                    access_map_rA->iteration_domain[loop].second,
                    access_map_rA->step_size[loop]);

                ctx->pushVar(iter_var);
                access_map_rA->setPinIterVar(loop, iter_var);
                access_map_rB->setPinIterVar(loop, iter_var);
                access_map_acc->setPinIterVar(loop, iter_var);
            }

            // Generate loop
            kernel +=
                fmt::format("{}for (int {} = {}; {} < {}; {} += {}) {{\n",
                            std::string(indient, ' '),
                            access_map_rA->pin_iter_vars[loop].value()->name,
                            access_map_rA->iteration_domain[loop].first,
                            access_map_rA->pin_iter_vars[loop].value()->name,
                            access_map_rA->iteration_domain[loop].second,
                            access_map_rA->pin_iter_vars[loop].value()->name,
                            access_map_rA->step_size[loop]);
            indient += 4;
        }

        std::string rA_access = rA->getBufferName().value();
        std::string rB_access = rB->getBufferName().value();
        std::string acc_access = acc->getBufferName().value();

        for (int access_loop = 0; access_loop < access_map_rA->getAccessDims();
             access_loop++) {
            auto rA_access_dim = access_map_rA->access_pattern[access_loop];
            for (int loop = 0; loop < access_map_rA->getLoops(); loop++) {
                if (rA_access_dim[loop] != 0)
                    rA_access += fmt::format(
                        "[{} * {}]", rA_access_dim[loop],
                        access_map_rA->pin_iter_vars[loop].value()->name);
            }
        }

        for (int access_loop = 0; access_loop < access_map_rB->getAccessDims();
             access_loop++) {
            auto rB_access_dim = access_map_rB->access_pattern[access_loop];
            for (int loop = 0; loop < access_map_rB->getLoops(); loop++) {
                if (rB_access_dim[loop] != 0)
                    rB_access += fmt::format(
                        "[{} * {}]", rB_access_dim[loop],
                        access_map_rB->pin_iter_vars[loop].value()->name);
            }
        }

        for (int access_loop = 0; access_loop < access_map_acc->getAccessDims();
             access_loop++) {
            auto acc_access_dim = access_map_acc->access_pattern[access_loop];
            for (int loop = 0; loop < access_map_acc->getLoops(); loop++) {
                if (acc_access_dim[loop] != 0)
                    acc_access += fmt::format(
                        "[{} * {}]", acc_access_dim[loop],
                        access_map_acc->pin_iter_vars[loop].value()->name);
            }
        }

        // TODO: Use macro kernel instead of hardcoding the kernel.
        kernel +=
            fmt::format("{}gemm({}, {}, {});\n", std::string(indient, ' '),
                        rA_access, rB_access, acc_access);

        // Close loop parenthesis.
        for (auto loop = 0; loop < access_map_rA->getLoops(); loop++) {
            indient -= 4;
            kernel += fmt::format("{}}}\n", std::string(indient, ' '));
        }
        return kernel;
    }

}  // namespace tiledkernel