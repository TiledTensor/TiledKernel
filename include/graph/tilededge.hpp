#pragma once
#include "mem_level.hpp"
#include "tiledbuffer.hpp"
#include "microop.hpp"
#include "platform.hpp"
#include <set>
#include <string>

namespace tiledkernel::graph {

    class TiledEdge {
       public:
        TiledEdge(std::set<TiledBuffer> inputs, std::set<TiledBuffer> outputs)
            : inputs(inputs), outputs(outputs) {}

        std::set<TiledBuffer> getInputs() { return inputs; }

        std::set<TiledBuffer> getOutputs() { return outputs; }

        std::string map(Platform platform);

       protected:
        std::set<TiledBuffer> inputs;
        std::set<TiledBuffer> outputs;
    };
};  // namespace tiledkernel::graph