#include "context.hpp"

namespace tiledkernel {

    TiledContext::TiledContext() {}

    void TiledContext::pushVar(std::shared_ptr<Var> var) {
        vars.push_back(var);
    }
}  // namespace tiledkernel