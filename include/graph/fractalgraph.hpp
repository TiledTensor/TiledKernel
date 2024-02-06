#include "graph/fractaledge.hpp"
#include "graph/fractalnode.hpp"
#include "mem_level.hpp"

namespace fractalkernel::graph {
    class FractalGraph {
       public:
        std::string map();

       protected:
        std::vector<FractalOutEdge> out_edges;
        std::vector<FractalInEdge> in_edges;
        MemoryLevel mem_level;
        std::vector<FractalMicroOp> nodes;
    };
}  // namespace fractalkernel::graph