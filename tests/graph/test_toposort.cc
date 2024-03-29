#include "graph/tiledgraph.hpp"
#include "graph/tilednode.hpp"
#include "graph/tilededge.hpp"
#include "type/data_type.hpp"
#include "tiledbuffer.hpp"
#include "op.hpp"

#include <gtest/gtest.h>
#include <fmtlog.h>

using namespace tiledkernel;
using namespace tiledkernel::type;
using namespace tiledkernel::graph;

TEST(Graph, tiledgraph_topo_rf_gemm_graph) {
    fmtlog::setLogLevel(fmtlog::LogLevel::INF);
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
    auto sA_rA_edge = std::make_shared<TiledEdge>("sA_rA_edge");
    auto sB_rB_edge = std::make_shared<TiledEdge>("sB_rB_edge");
    auto rA_gemm_edge = std::make_shared<TiledEdge>("rA_gemm_edge");
    auto rB_gemm_edge = std::make_shared<TiledEdge>("rB_gemm_edge");
    auto gemm_acc_edge = std::make_shared<TiledEdge>("gemm_acc_edge");
    auto acc_sC_edge = std::make_shared<TiledEdge>("acc_sC_edge");

    // Define nodes
    auto sA_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Shared, TiledNodeData{sA}, "sA_node",
        std::vector<EdgePtr>{}, std::vector<EdgePtr>{sA_rA_edge});

    auto sB_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Shared, TiledNodeData{sB}, "sB_node",
        std::vector<EdgePtr>{}, std::vector<EdgePtr>{sB_rB_edge});

    auto rA_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::RF, TiledNodeData{rA}, "rA_node",
        std::vector<EdgePtr>{sA_rA_edge}, std::vector<EdgePtr>{rA_gemm_edge});

    auto rB_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::RF, TiledNodeData{rB}, "rB_node",
        std::vector<EdgePtr>{sB_rB_edge}, std::vector<EdgePtr>{rB_gemm_edge});

    auto gemm_node = std::make_shared<TiledNode>(
        NodeType::Operator, MemoryLevel::RF, TiledNodeData{gemm}, "gemm_node",
        std::vector<TiledEdge::Pointer>{rA_gemm_edge, rB_gemm_edge},
        std::vector<TiledEdge::Pointer>{gemm_acc_edge});

    auto acc_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::RF, TiledNodeData{acc}, "acc_node",
        std::vector<EdgePtr>{gemm_acc_edge}, std::vector<EdgePtr>{acc_sC_edge});

    auto sC_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Shared, TiledNodeData{sC}, "sC_node",
        std::vector<EdgePtr>{acc_sC_edge}, std::vector<EdgePtr>{});

    auto rf_gemm_graph = TiledGraph(
        MemoryLevel::RF, "rf_gemm_graph",
        std::vector<NodePtr>{rA_node, rB_node, gemm_node, acc_node},
        std::vector<EdgePtr>{sA_rA_edge, sB_rB_edge},
        std::vector<EdgePtr>{acc_sC_edge},
        std::vector<EdgePtr>{rA_gemm_edge, rB_gemm_edge, gemm_acc_edge});

    auto sorted_nodes = rf_gemm_graph.topoSort();

    ASSERT_EQ(sorted_nodes.size(), 4);
    ASSERT_EQ(sorted_nodes[0], rB_node);
    ASSERT_EQ(sorted_nodes[1], rA_node);
    ASSERT_EQ(sorted_nodes[2], gemm_node);
    ASSERT_EQ(sorted_nodes[3], acc_node);
}

TEST(Graph, test_toposort_shared_gemm_graph) {
    // Build a Shared Gemm graph

    std::cout << "Shared GEMM Graph Example:" << std::endl;
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

    auto gA = std::make_shared<TiledBuffer>("gA", MemoryLevel::Global,
                                            DataType::Float32);
    auto gB = std::make_shared<TiledBuffer>("gB", MemoryLevel::Global,
                                            DataType::Float32);
    auto gC = std::make_shared<TiledBuffer>("gC", MemoryLevel::Global,
                                            DataType::Float32);

    // Define edges
    auto sA_rA_edge = std::make_shared<TiledEdge>("sA_rA_edge");
    auto sB_rB_edge = std::make_shared<TiledEdge>("sB_rB_edge");
    auto rA_gemm_edge = std::make_shared<TiledEdge>("rA_gemm_edge");
    auto rB_gemm_edge = std::make_shared<TiledEdge>("rB_gemm_edge");
    auto gemm_acc_edge = std::make_shared<TiledEdge>("gemm_acc_edge");
    auto acc_sC_edge = std::make_shared<TiledEdge>("acc_sC_edge");

    auto gA_sA_edge = std::make_shared<TiledEdge>("gA_sA_edge");
    auto gB_sB_edge = std::make_shared<TiledEdge>("gB_sB_edge");
    auto sC_gC_edge = std::make_shared<TiledEdge>("sC_gC_edge");

    // Define nodes
    auto sA_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Shared, TiledNodeData{sA}, "sA_node",
        std::vector<EdgePtr>{gA_sA_edge}, std::vector<EdgePtr>{sA_rA_edge});

    auto sB_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Shared, TiledNodeData{sB}, "sB_node",
        std::vector<EdgePtr>{gB_sB_edge}, std::vector<EdgePtr>{sB_rB_edge});

    auto rA_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::RF, TiledNodeData{rA}, "rA_node",
        std::vector<EdgePtr>{sA_rA_edge}, std::vector<EdgePtr>{rA_gemm_edge});

    auto rB_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::RF, TiledNodeData{rB}, "rB_node",
        std::vector<EdgePtr>{sB_rB_edge}, std::vector<EdgePtr>{rB_gemm_edge});

    auto gemm_node = std::make_shared<TiledNode>(
        NodeType::Operator, MemoryLevel::RF, TiledNodeData{gemm}, "gemm_node",
        std::vector<TiledEdge::Pointer>{rA_gemm_edge, rB_gemm_edge},
        std::vector<TiledEdge::Pointer>{gemm_acc_edge});

    auto acc_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::RF, TiledNodeData{acc}, "acc_node",
        std::vector<EdgePtr>{gemm_acc_edge}, std::vector<EdgePtr>{acc_sC_edge});

    auto sC_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Shared, TiledNodeData{sC}, "sC_node",
        std::vector<EdgePtr>{acc_sC_edge}, std::vector<EdgePtr>{sC_gC_edge});

    auto gA_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Global, TiledNodeData{gA}, "gA_node",
        std::vector<EdgePtr>{}, std::vector<EdgePtr>{gA_sA_edge});

    auto gB_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Global, TiledNodeData{gB}, "gB_node",
        std::vector<EdgePtr>{}, std::vector<EdgePtr>{gB_sB_edge});

    auto gC_node = std::make_shared<TiledNode>(
        NodeType::Buffer, MemoryLevel::Global, TiledNodeData{gC}, "gC_node",
        std::vector<EdgePtr>{sC_gC_edge}, std::vector<EdgePtr>{});

    auto rf_gemm_graph = std::make_shared<TiledGraph>(
        MemoryLevel::RF, "rf_gemm_graph",
        std::vector<NodePtr>{rA_node, rB_node, gemm_node, acc_node},
        std::vector<EdgePtr>{sA_rA_edge, sB_rB_edge},
        std::vector<EdgePtr>{acc_sC_edge},
        std::vector<EdgePtr>{rA_gemm_edge, rB_gemm_edge, gemm_acc_edge});

    auto rf_gemm_node = std::make_shared<TiledNode>(
        NodeType::Task, MemoryLevel::RF, TiledNodeData{rf_gemm_graph},
        "rf_gemm_node", std::vector<EdgePtr>{sA_rA_edge, sB_rB_edge},
        std::vector<EdgePtr>{acc_sC_edge});

    std::cout << "Connect Shared GEMM Graph:" << std::endl;
    auto shared_gemm_graph = std::make_shared<TiledGraph>(
        MemoryLevel::Shared, "shared_gemm_graph",
        std::vector<NodePtr>{sA_node, sB_node, sC_node, rf_gemm_node},
        std::vector<EdgePtr>{gA_sA_edge, gB_sB_edge},
        std::vector<EdgePtr>{sC_gC_edge},
        std::vector<EdgePtr>{sA_rA_edge, sB_rB_edge, acc_sC_edge});

    auto sorted_nodes = shared_gemm_graph->topoSort();

    ASSERT_EQ(sorted_nodes.size(), 4);
    ASSERT_EQ(sorted_nodes[0], sB_node);
    ASSERT_EQ(sorted_nodes[1], sA_node);
    ASSERT_EQ(sorted_nodes[2], rf_gemm_node);
    ASSERT_EQ(sorted_nodes[3], sC_node);
}