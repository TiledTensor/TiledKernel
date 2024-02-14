#pragma once
#include "type/data_type.hpp"
#include "id.hpp"
#include <string>
#include <memory>
#include <vector>

namespace tiledkernel {
    class Var {
       public:
        Var(type::DataType dtype) : dtype(dtype) { id = ID::make(); }

        Var(std::string name, type::DataType dtype) : name(name), dtype(dtype) {
            id = ID::make();
        }

        bool operator==(const Var& other) const { return id == other.id; }

        std::string name;
        type::DataType dtype;
        ID id;

        using Pointer = std::shared_ptr<Var>;
    };

    class IterVar : public Var {
       public:
        IterVar(type::DataType dtype, int32_t start, int32_t end, int32_t step)
            : Var(dtype), start(start), end(end), step(step) {
            const std::vector<std::string> ITERS = {"i", "j", "k", "l", "m",
                                                    "n", "o", "p", "q", "r",
                                                    "s", "t", "u", "v", "w"};
            name = ITERS[id % ITERS.size()] + std::to_string(id);
        }

        IterVar(std::string name, type::DataType dtype, int32_t start,
                int32_t end, int32_t step)
            : Var(name, dtype), start(start), end(end), step(step) {}

        std::string getName() const { return name; }

        // std::string map();

        int32_t start;
        int32_t end;
        int32_t step;

        using Pointer = std::shared_ptr<IterVar>;
    };
};  // namespace tiledkernel