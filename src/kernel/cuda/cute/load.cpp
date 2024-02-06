#include "kernel/cuda/cute/load.hpp"

namespace tiledkernel::kernel::cuda::cute {
    // TODO: Implement this function.
    std::string load(BufferPtr a, BufferPtr b) {
        if (a->mem_level == MemoryLevel::Shared &&
            b->mem_level == MemoryLevel::RF) {
            return a->name + " copy(" + ");";
        }
    }
}  // namespace tiledkernel::kernel::cuda::cute