#pragma once
#include "type/data_type.hpp"
#include "id.hpp"
#include <string>

namespace tiledkernel {
    class Var {
       public:
        Var(std::string name, type::DataType dtype) : name(name), dtype(dtype) {
            id = ID::make();
        }

        bool operator==(const Var& other) const { return id == other.id; }

        std::string name;
        type::DataType dtype;
        ID id;
    };

    class IterVar : public Var {
       public:
        IterVar(std::string name, type::DataType dtype, int32_t start,
                int32_t end, int32_t step)
            : Var(name, dtype), start(start), end(end), step(step) {}

        std::string map();

        int32_t start;
        int32_t end;
        int32_t step;
    };
};  // namespace tiledkernel