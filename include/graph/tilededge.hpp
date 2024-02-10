#pragma once
#include "mem_level.hpp"
#include "tiledbuffer.hpp"
#include "microop.hpp"
#include "platform.hpp"
#include "access_map.hpp"
#include <vector>
#include <string>

namespace tiledkernel::graph {

    class TiledEdge {
       public:
        TiledEdge(std::shared_ptr<TiledBuffer> input,
                  std::shared_ptr<TiledBuffer> output,
                  std::shared_ptr<AccessMap> access_map)
            : input(input), output(output), access_map(access_map) {}

        std::shared_ptr<TiledBuffer> getInput() { return input; }

        std::shared_ptr<TiledBuffer> getOutput() { return output; }

        std::string map(Platform platform);

       protected:
        std::shared_ptr<TiledBuffer> input;
        std::shared_ptr<TiledBuffer> output;
        std::shared_ptr<AccessMap> access_map;
    };
};  // namespace tiledkernel::graph