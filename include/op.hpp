#pragma once
#include <vector>

namespace tiledkernel {
    enum OpType { Add, Sub, Mul, Div, Gemm, Null };
    class Operator {
       public:
        Operator(OpType op_type = OpType::Add);

        OpType getOpType() { return op_type; }

       private:
        OpType op_type;
    };

}  // namespace tiledkernel