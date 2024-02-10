#pragma once
#include "context.hpp"

namespace tiledkernel {
    void TiledContext::pushVar(std::shared_ptr<Var> var) {
        vars.push_back(var);
    }
}