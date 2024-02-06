#include "mem_level.hpp"
#include "tiledbuffer.hpp"
#include "microop.hpp"
#include <set>
#include <string>

namespace tiledkernel::graph {
    // Edge types
    enum EdgeType { Input, Output };

    // FractalEdge class
    template <class Input, class Output>
    class TiledEdge {
       public:
        TiledEdge(std::set<Input> inputs, std::set<Output> outputs,
                  MemoryLevel mem_level, EdgeType edge_type) {
            this->inputs = inputs;
            this->outputs = outputs;
            this->mem_level = mem_level;
            this->edge_type = edge_type;
        }

        std::set<Input> getInputs() { return inputs; }

        std::set<Output> getOutputs() { return outputs; }

        MemoryLevel getMmemLevel() { return mem_level; }

        EdgeType getEdgeType() { return edge_type; }

        std::string map();

       protected:
        std::set<Input> inputs;
        std::set<Output> outputs;
        MemoryLevel mem_level;
        EdgeType edge_type;
    };

    using TiledOutEdge = TiledEdge<TiledMicroOp, TiledBuffer>;
    using TiledInEdge = TiledEdge<TiledBuffer, TiledMicroOp>;
};  // namespace tiledkernel::graph