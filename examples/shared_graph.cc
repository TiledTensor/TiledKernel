#include "graph/tiledgraph.hpp"
#include "graph/tilednode.hpp"
#include "graph/tilededge.hpp"
#include "type/data_type.hpp"
#include "tiledbuffer.hpp"
#include "generator.hpp"
#include "op.hpp"
#include <iostream>
#include <fmtlog.h>

using namespace tiledkernel;
using namespace tiledkernel::type;
using namespace tiledkernel::graph;

int main() {
    fmtlog::setLogLevel(fmtlog::LogLevel::DBG);
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

    auto rf_sorted_nodes = rf_gemm_graph->topoSort();
    std::cout << "ID\tName" << std::endl;
    for (auto node : rf_sorted_nodes) {
        std::cout << node->id << "\t" << node->name << std::endl;
    }

    auto rf_gemm_node = std::make_shared<TiledNode>(
        NodeType::Task, MemoryLevel::RF, TiledNodeData{rf_gemm_graph},
        "rf_gemm_node", std::vector<EdgePtr>{sA_rA_edge, sB_rB_edge},
        std::vector<EdgePtr>{acc_sC_edge});

    // Define Access Map
    auto rA_gemm_access_map_i = std::make_shared<AccessMap>(
        1, 1, std::vector<std::vector<int32_t>>{std::vector<int32_t>{1}},
        std::vector<std::pair<int32_t, int32_t>>{std::make_pair(0, 10)},
        std::vector<int32_t>{0});

    auto rB_gemm_access_map_i = std::make_shared<AccessMap>(
        1, 1, std::vector<std::vector<int32_t>>{std::vector<int32_t>{1}},
        std::vector<std::pair<int32_t, int32_t>>{std::make_pair(0, 10)},
        std::vector<int32_t>{0});

    auto gemm_acc_access_map_i = std::make_shared<AccessMap>(
        1, 1, std::vector<std::vector<int32_t>>{std::vector<int32_t>{0}},
        std::vector<std::pair<int32_t, int32_t>>{std::make_pair(0, 10)},
        std::vector<int32_t>{0});

    rA_gemm_edge->setAccessMapI(rA_gemm_access_map_i);
    rB_gemm_edge->setAccessMapI(rB_gemm_access_map_i);
    gemm_acc_edge->setAccessMapI(gemm_acc_access_map_i);

    std::cout << "Connect Shared GEMM Graph:" << std::endl;
    auto shared_gemm_graph = std::make_shared<TiledGraph>(
        MemoryLevel::Shared, "shared_gemm_graph",
        std::vector<NodePtr>{sA_node, sB_node, sC_node, rf_gemm_node},
        std::vector<EdgePtr>{gA_sA_edge, gB_sB_edge},
        std::vector<EdgePtr>{sC_gC_edge},
        std::vector<EdgePtr>{sA_rA_edge, sB_rB_edge, acc_sC_edge});

    auto sorted_nodes = shared_gemm_graph->topoSort();

    std::cout << "ID\tName" << std::endl;
    for (auto node : sorted_nodes) {
        std::cout << node->id << "\t" << node->name << std::endl;
    }

    auto generator = std::make_shared<TiledGenerator>();

    auto kernel = generator->emit(Platform::Cute, shared_gemm_graph);

    std::cout << "Generate kernel:" << std::endl << kernel << std::endl;
}