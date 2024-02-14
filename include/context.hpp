#pragma once
#include "var.hpp"
#include <memory>

namespace tiledkernel {
    class TiledContext {
       public:
        TiledContext();
        ~TiledContext() = default;

        void pushVar(std::shared_ptr<Var> var);

        std::vector<std::shared_ptr<Var>> vars;

        using Pointer = std::shared_ptr<TiledContext>;
    };

}  // namespace tiledkernel