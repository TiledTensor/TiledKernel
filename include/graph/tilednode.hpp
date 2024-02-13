#pragma once
#include "mem_level.hpp"
#include "tiledbuffer.hpp"
#include "id.hpp"
#include "op.hpp"
#include <memory>
#include <string>
#include <variant>

namespace tiledkernel::graph {
    class TiledEdge;
    class TiledGraph;

    enum class NodeType { Null, Buffer, Task, Operator };

    using TiledNodeData =
        std::variant<std::shared_ptr<Operator>, std::shared_ptr<TiledBuffer>,
                     std::shared_ptr<TiledGraph>, std::monostate>;

    class TiledNode {
       public:
        ID id;
        int32_t in_degrees;
        NodeType node_type;
        MemoryLevel mem_level;
        TiledNodeData data;
        std::string name;

        std::vector<std::shared_ptr<TiledEdge>> in_edges;
        std::vector<std::shared_ptr<TiledEdge>> out_edges;
        std::vector<std::shared_ptr<TiledNode>> predecessors;
        std::vector<std::shared_ptr<TiledNode>> successors;

        TiledNode(NodeType node_type = NodeType::Null,
                  MemoryLevel mem_level = MemoryLevel::RF,
                  TiledNodeData data = TiledNodeData{std::monostate{}},
                  std::string name = "",
                  std::vector<std::shared_ptr<TiledEdge>> in_edges = {},
                  std::vector<std::shared_ptr<TiledEdge>> out_edges = {});
    };

    using NodePtr = std::shared_ptr<TiledNode>;

}  // namespace tiledkernel::graph