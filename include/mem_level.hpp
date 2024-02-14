#pragma once
namespace tiledkernel {
    enum MemoryLevel { RF = 1, Shared = 2, Global = 3 };

    // bool operator>(MemoryLevel a, MemoryLevel b) {
    //     return static_cast<int>(a) > static_cast<int>(b);
    // }
};  // namespace tiledkernel