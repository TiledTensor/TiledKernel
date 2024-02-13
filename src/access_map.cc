#include "type/data_type.hpp"
#include "access_map.hpp"
#include "var.hpp"
#include <fmt/core.h>

namespace tiledkernel {
    // std::string AccessMap::map(Platform platform, BufferPtr a, BufferPtr b) {
    //     std::string kernel;
    //     std::vector<IterVar> iter_vars;
    //     uint32_t indient = 0;
    //     const std::string ITERNAME[] = {"i", "j", "k", "l", "m", "n"};
    //     if (platform == Platform::Cpu) {
    //         // Generate loop nesting based on `loop_depth`.
    //         for (int i = 0; i < loop_depth; i++) {
    //             // Create a new iterator.
    //             IterVar iter_var =
    //                 IterVar(ITERNAME[i], type::DataType::Int32,
    //                         iteration_domain[i].first,
    //                         iteration_domain[i].second, step_size[i]);
    //             iter_vars.push_back(iter_var);

    //             kernel += std::string(indient, ' ');
    //             kernel += iter_var.map();
    //             indient += 4;
    //         }
    //         // Generate load kernel based on `access_pattern`.
    //         kernel += std::string(indient, ' ');

    //         // Close loop parenthesis.
    //         for (int i = 0; i < loop_depth; i++) {
    //             indient -= 4;
    //             kernel += std::string(indient, ' ');
    //             kernel += "}\n";
    //         }

    //     } else if (platform == Platform::Cute) {
    //         // kernel = kernel + load(a, b);
    //     } else if (platform == Platform::Triton) {
    //         // kernel = kernel + load(a, b);
    //     }
    //     return kernel;
    // }
}  // namespace tiledkernel