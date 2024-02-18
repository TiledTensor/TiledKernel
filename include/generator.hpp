#pragma once
#include "graph/tiledgraph.hpp"
#include "platform.hpp"
#include "context.hpp"
#include "access_map.hpp"
#include "var.hpp"

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

        // Emit code for the given platform.
        std::string emit_cute(TiledGraph::Pointer graph);

        // Emit code for the given memory hierarchy graph.
        std::string emit_rf_cute(TiledGraph::Pointer graph);
        std::string emit_shared_cute(TiledGraph::Pointer graph);
        std::string emit_global_cute(TiledGraph::Pointer graph);

        std::string emit_rf_cute_gemm(TiledNode::Pointer node);

        std::string generate_loop(IterVar::Pointer iter_var, std::string body);
        std::string generate_access_map(AccessMap::Pointer access_map);
    };
}  // namespace tiledkernel