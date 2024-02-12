#include "graph/tiledgraph.hpp"
#include "graph/tilednode.hpp"
#include "graph/tilededge.hpp"
#include "type/data_type.hpp"
#include "tiledbuffer.hpp"
#include "op.hpp"

#include <gtest/gtest.h>

using namespace tiledkernel;
using namespace tiledkernel::type;
using namespace tiledkernel::graph;

TEST(Graph, tiledgraph_topo) {
    // Build a RF Gemm graph

    // Define buffers
    auto sA = std::make_shared<TiledBuffer>("sA", MemoryLevel::Shared,
                                            DataType::Float32);
    auto sB = std::make_shared<TiledBuffer>("sB", MemoryLevel::Shared,
                                            DataType::Float32);
    auto sC = std::make_shared<TiledBuffer>("sC", MemoryLevel::Shared,
                                            DataType::Float32);

    auto gemm = std::make_shared<Operator>(OpType::Gemm);

    auto rA =
        std::make_shared<TiledBuffer>("rA", MemoryLevel::RF, DataType::Float32);
    auto rB =
        std::make_shared<TiledBuffer>("rB", MemoryLevel::RF, DataType::Float32);
    auto acc = std::make_shared<TiledBuffer>("acc", MemoryLevel::RF,
                                             DataType::Float32);

    // Define edges
    auto sA_rA_edge = std::make_shared<TiledEdge>();
    auto sB_rB_edge = std::make_shared<TiledEdge>();
    auto rA_gemm_edge = std::make_shared<TiledEdge>();
    auto rB_gemm_edge = std::make_shared<TiledEdge>();
    auto gemm_acc_edge = std::make_shared<TiledEdge>();
    auto acc_sC_edge = std::make_shared<TiledEdge>();

    // Define nodes
    auto sA_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Shared, TiledNodeData{sA},
        std::vector<EdgePtr>{}, std::vector<EdgePtr>{sA_rA_edge});

    auto sB_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Shared, TiledNodeData{sB},
        std::vector<EdgePtr>{}, std::vector<EdgePtr>{sB_rB_edge});

    auto rA_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::RF, TiledNodeData{rA},
        std::vector<EdgePtr>{sA_rA_edge}, std::vector<EdgePtr>{rA_gemm_edge});

    auto rB_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::RF, TiledNodeData{rB},
        std::vector<EdgePtr>{sB_rB_edge}, std::vector<EdgePtr>{rB_gemm_edge});

    auto gemm_node = std::make_shared<TiledNode>(
        NodeType::Operator, MemoryLevel::RF, TiledNodeData{gemm},
        std::vector<TiledEdge::Pointer>{rA_gemm_edge, rB_gemm_edge},
        std::vector<TiledEdge::Pointer>{gemm_acc_edge});

    auto acc_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::RF, TiledNodeData{acc},
        std::vector<EdgePtr>{gemm_acc_edge}, std::vector<EdgePtr>{acc_sC_edge});

    auto sC_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Shared, TiledNodeData{sC},
        std::vector<EdgePtr>{acc_sC_edge}, std::vector<EdgePtr>{});

    auto rf_gemm_graph = TiledGraph(
        MemoryLevel::RF,
        std::vector<NodePtr>{sA_node, sB_node, rA_node, rB_node, gemm_node,
                             acc_node, sC_node},
        std::vector<EdgePtr>{sA_rA_edge, sB_rB_edge},
        std::vector<EdgePtr>{rA_gemm_edge, rB_gemm_edge, gemm_acc_edge});

    auto sorted_nodes = rf_gemm_graph.topoSort();

    for (auto node : sorted_nodes) {
        std::cout << node->id << std::endl;
    }
}