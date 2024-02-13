#include "generator.hpp"

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

    std::string TiledGenerator::emit_cute(TiledGraph::Pointer graph) {}

}  // namespace tiledkernel