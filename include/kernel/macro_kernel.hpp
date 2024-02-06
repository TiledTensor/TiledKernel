#pragma once
#include <string>
#include <memory>
#include "context.hpp"

namespace tiledkernel::kernel {
    class TiledMacroKernel {
       public:
        std::string generate();

        std::shared_ptr<TiledContext> ctx;
    };
}  // namespace tiledkernel::kernel