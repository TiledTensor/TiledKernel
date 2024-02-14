#pragma once
#include "graph/tiledgraph.hpp"
#include "platform.hpp"
#include "context.hpp"

namespace tiledkernel {
    using namespace tiledkernel::graph;
    class TiledGenerator {
       public:
        TiledGenerator();
        ~TiledGenerator() = default;

        TiledContext::Pointer getContext() { return ctx; }
        // Emit code for the given graph
        std::string emit(Platform platform, TiledGraph::Pointer graph);

       private:
        TiledContext::Pointer ctx;

        std::string emit_cute(TiledGraph::Pointer graph);
        std::string emit_rf_cute(TiledGraph::Pointer graph);
        std::string emit_rf_cute_gemm(TiledNode::Pointer node);
    };
}  // namespace tiledkernel