#include "mem_level.hpp"
#include "microop.hpp"
#include <memory>
#include <string>

namespace fractalkernel::graph {
    template <class MicroOp>
    class FractalNode {
       public:
        FractalNode(std::shared_ptr<MicroOp> micro_op);
        std::string map();

       protected:
        std::shared_ptr<MicroOp> micro_op;
        MemoryLevel mem_level;
        // TODO: InEdges, OutEdges
    };

    using FractalMicroOpNode = FractalNode<FractalMicroOp>;
}  // namespace fractalkernel::graph