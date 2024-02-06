#pragma once
#include "mem_level.hpp"
#include "microop.hpp"
#include <memory>
#include <string>

namespace tiledkernel::graph {
    template <class MicroOp>
    class TiledNode {
       public:
        TiledNode(std::shared_ptr<MicroOp> micro_op);
        std::string map();

       protected:
        std::shared_ptr<MicroOp> micro_op;
        MemoryLevel mem_level;
        // TODO: InEdges, OutEdges
    };

    using TiledMicroOpNode = TiledNode<TiledMicroOp>;
}  // namespace tiledkernel::graph