#pragma once
#include <vector>

namespace tiledkernel {
    enum OpType { Add, Sub, Mul, Div, Gemm };
    class Operator {
       public:
        Operator(OpType op_type = OpType::Add);

        OpType op_type;
    };

}  // namespace tiledkernel