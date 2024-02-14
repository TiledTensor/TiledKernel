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
        auto access_map_rA = gemm_in_edges[0]->getAccessMapI();
        auto access_map_rB = gemm_in_edges[1]->getAccessMapI();
        auto access_map_acc = gemm_out_edges[0]->getAccessMapI();

        // TODO: Check if the access map is valid.

        // TODO: Use macro kernel instead of hardcoding the kernel.
        kernel += fmt::format(
            "gemm({}, {}, {});\n", rA->getBufferName().value(),
            rB->getBufferName().value(), acc->getBufferName().value());
        return kernel;
    }

}  // namespace tiledkernel