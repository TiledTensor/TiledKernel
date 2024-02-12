#include "graph/tilednode.hpp"
#include "graph/tilededge.hpp"

namespace tiledkernel::graph {
    TiledNode::TiledNode(NodeType node_type, MemoryLevel mem_level,
                         TiledNodeData data,
                         std::vector<std::shared_ptr<TiledEdge>> in_edges,
                         std::vector<std::shared_ptr<TiledEdge>> out_edges)
        : node_type(node_type),
          mem_level(mem_level),
          data(data),
          in_edges(in_edges),
          out_edges(out_edges) {
        id = ID::make();
        in_degrees = in_edges.size();
    }
}  // namespace tiledkernel::graph