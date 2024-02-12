#pragma once
#include "type/data_type.hpp"
#include "mem_level.hpp"
#include <string>

namespace tiledkernel {
    class TiledBuffer {
       public:
        TiledBuffer(std::string name, MemoryLevel mem_level,
                    type::DataType dtype);

        ~TiledBuffer() = default;

        std::string name;
        MemoryLevel mem_level;
        type::DataType dtype;
    };

    using BufferPtr = std::shared_ptr<TiledBuffer>;
}  // namespace tiledkernel