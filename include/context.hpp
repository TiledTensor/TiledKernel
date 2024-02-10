#pragma once
#include "var.hpp"
#include <memory>

namespace tiledkernel {
    class TiledContext {
       public:
        TiledContext();
        ~TiledContext();

        void pushVar(std::shared_ptr<Var> var);

        std::vector<std::shared_ptr<Var>> vars;
    };

    using ContextPtr = std::shared_ptr<TiledContext>;
}  // namespace tiledkernel