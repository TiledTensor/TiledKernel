#include "id.hpp"

namespace tiledkernel {
    std::atomic_uint64_t ID::globalIdCnt_ = 1;
}  // namespace tiledkernel