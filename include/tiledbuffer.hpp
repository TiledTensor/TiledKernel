#pragma once
#include "mem_level.hpp"
#include <string>

namespace tiledkernel {
    class TiledBuffer {
       public:
        TiledBuffer(std::string name, MemoryLevel mem_level)
            : name(name), mem_level(mem_level) {}

        std::string name;
        MemoryLevel mem_level;
    };

    using BufferPtr = std::shared_ptr<TiledBuffer>;
}  // namespace tiledkernel