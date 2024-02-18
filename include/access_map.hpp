#pragma once
#include "tiledbuffer.hpp"
#include "platform.hpp"
#include "context.hpp"
#include "var.hpp"
#include <vector>
#include <string>
#include <memory>
#include <optional>

namespace tiledkernel {
    class AccessMap {
       public:
        AccessMap(size_t loop_depth, size_t access_dims,
                  std::vector<std::vector<int32_t>> access_pattern,
                  std::vector<std::pair<int32_t, int32_t>> iteration_domain,
                  std::vector<int32_t> offset)
            : loop_depth(loop_depth),
              access_dims(access_dims),
              access_pattern(access_pattern),
              iteration_domain(iteration_domain),
              offset(offset) {
            pin_iter_vars.resize(loop_depth);
            for (auto& iter_var : pin_iter_vars) {
                iter_var = std::nullopt;
            }
        }

        void setPinIterVar(size_t loop_index,
                           std::shared_ptr<IterVar> iter_var) {
            pin_iter_vars[loop_index] = iter_var;
        }

        bool hasPinIterVar(size_t loop_index) {
            return pin_iter_vars[loop_index].has_value();
        }

        size_t getLoops() { return loop_depth; }

        size_t getAccessDims() { return access_dims; }

        // std::string map(Platform platform, BufferPtr a, BufferPtr b);

        size_t loop_depth;
        size_t access_dims;
        // row: access_dims, col: loop_depth
        std::vector<std::vector<int32_t>> access_pattern;
        // loop_depth
        std::vector<std::pair<int32_t, int32_t>> iteration_domain;
        // loop_depth
        // std::vector<int32_t> step_size;
        std::vector<int32_t> offset;

        std::vector<std::optional<std::shared_ptr<IterVar>>> pin_iter_vars;

        using Pointer = std::shared_ptr<AccessMap>;
    };
}  // namespace tiledkernel