#include "tiledbuffer.hpp"

namespace tiledkernel {
    TiledBuffer::TiledBuffer(std::string name, MemoryLevel mem_level,
                             type::DataType dtype)
        : name(name), mem_level(mem_level), dtype(dtype) {}
}  // namespace tiledkernel