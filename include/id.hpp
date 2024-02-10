#pragma once
#include <string>

namespace tiledkernel {

    class ID {
        uint64_t id_;

        static std::atomic_uint64_t globalIdCnt_;
        explicit ID(uint64_t id) : id_(id) {}

       public:
        ID() : id_(0) {}
        ID(std::nullopt_t) : id_(0) {}

        static ID make() { return ID(globalIdCnt_++); }
        static ID make(uint64_t id) { return ID(id); }

        bool isValid() const { return id_ != 0; }

        operator uint64_t() const { return id_; }
    };
};  // namespace tiledkernel