#include "graph/tilededge.hpp"
#include "graph/tilednode.hpp"
#include "mem_level.hpp"

namespace tiledkernel::graph {
    class FractalGraph {
       public:
        std::string map();

       protected:
        std::vector<TiledOutEdge> out_edges;
        std::vector<TiledInEdge> in_edges;
        MemoryLevel mem_level;
        std::vector<TiledMicroOp> nodes;
    };
}  // namespace tiledkernel::graph