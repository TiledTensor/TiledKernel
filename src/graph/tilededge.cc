#include "graph/tilededge.hpp"

namespace tiledkernel::graph {

    std::string TiledEdge::map(Platform platform) {
        std::string load_kernel;
        if (platform == Platform::Cpu) {
            // Add iterator.
            // Add load kernel.
        } else if (platform == Platform::Cute) {
        } else if (platform == Platform::Triton) {
        }
    }

};  // namespace tiledkernel::graph