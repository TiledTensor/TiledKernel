#include "access_map.hpp"
#include <fmt/core.h>

namespace tiledkernel {
    std::string AccessMap::map(Platform platform, BufferPtr a, BufferPtr b,
                               ContextPtr ctx) {
        std::string kernel;
        if (platform == Platform::Cpu) {
            // Generate loop nesting based on `loop_depth`.

            // Generate load kernel based on `access_pattern`.

        } else if (platform == Platform::Cute) {
            // kernel = kernel + load(a, b);
        } else if (platform == Platform::Triton) {
            // kernel = kernel + load(a, b);
        }
        return kernel;
    }
}  // namespace tiledkernel