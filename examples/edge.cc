#include "graph/tilededge.hpp"
#include "type/data_type.hpp"
#include "tiledbuffer.hpp"
#include "platform.hpp"
#include "access_map.hpp"
#include "context.hpp"
#include <memory>
#include <iostream>

using namespace tiledkernel;
using namespace tiledkernel::graph;
using namespace tiledkernel::type;

int main() {
    auto CTX = std::make_shared<TiledContext>();

    auto sA = std::make_shared<TiledBuffer>("sA", MemoryLevel::Shared,
                                            DataType::Float32);
    auto sB = std::make_shared<TiledBuffer>("sB", MemoryLevel::Shared,
                                            DataType::Float32);
    auto rA =
        std::make_shared<TiledBuffer>("rA", MemoryLevel::RF, DataType::Float32);
    auto rB =
        std::make_shared<TiledBuffer>("rB", MemoryLevel::RF, DataType::Float32);

    auto access_map_a = std::make_shared<AccessMap>(
        1, 1, std::vector<std::vector<int32_t>>{{1}},
        std::vector<std::pair<int32_t, int32_t>>{std::make_pair(0, 10)},
        std::vector<int32_t>{1}, std::vector<int32_t>{0});

    auto edge_a = std::make_shared<TiledEdge>(sA, rA, access_map_a);
    // auto edge_b = std::make_shared<TiledEdge>(sB, rB);

    auto load_kernel_a = edge_a->map(Platform::Cpu);
    // auto load_kernel_b = edge_b->map(Platform::Cute);
    std::cout << load_kernel_a << std::endl;
    // std::cout << load_kernel_b << std::endl;
}
