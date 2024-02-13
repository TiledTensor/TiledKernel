#pragma once
#include "graph/tiledgraph.hpp"
#include "platform.hpp"

namespace tiledkernel {
    using namespace tiledkernel::graph;
    class TiledGenerator {
       public:
        std::string emit(Platform platform, TiledGraph::Pointer graph);
    };
}  // namespace tiledkernel