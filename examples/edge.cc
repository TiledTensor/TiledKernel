#include "graph/tilededge.hpp"
#include "tiledbuffer.hpp"
#include "platform.hpp"
#include <memory>
#include <iostream>

using namespace tiledkernel;
using namespace tiledkernel::graph;

int main() {
    auto sA = std::make_shared<TiledBuffer>("sA", MemoryLevel::Shared);
    auto sB = std::make_shared<TiledBuffer>("sB", MemoryLevel::Shared);
    auto rA = std::make_shared<TiledBuffer>("rA", MemoryLevel::RF);
    auto rB = std::make_shared<TiledBuffer>("rB", MemoryLevel::RF);

    auto edge_a = std::make_shared<TiledEdge>(std::set<TiledBuffer>{*sA},
                                              std::set<TiledBuffer>{*rA});
    auto edge_b = std::make_shared<TiledEdge>(std::set<TiledBuffer>{*sB},
                                              std::set<TiledBuffer>{*rB});

    auto load_kernel_a = edge_a->map(Platform::Cute);
    auto load_kernel_b = edge_b->map(Platform::Cute);
    std::cout << load_kernel_a << std::endl;
    std::cout << load_kernel_b << std::endl;
}
