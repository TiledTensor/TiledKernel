#pragma once
#include "mem_level.hpp"
#include "id.hpp"
#include <vector>
#include <memory>

namespace tiledkernel::graph {
    class TiledNode;
    class TiledEdge;

    class TiledGraph {
       public:
        TiledGraph(MemoryLevel mem_level = MemoryLevel::RF,
                   std::string name = "",
                   std::vector<std::shared_ptr<TiledNode>> nodes = {},
                   std::vector<std::shared_ptr<TiledEdge>> in_edges = {},
                   std::vector<std::shared_ptr<TiledEdge>> out_edges = {},
                   std::vector<std::shared_ptr<TiledEdge>> intra_edges = {});

        MemoryLevel getMemLevel() { return mem_level; }

        void connect();

        void clear();

        std::vector<std::shared_ptr<TiledNode>> topoSort();

        bool isNodeExist(std::shared_ptr<TiledNode> node);

        using Pointer = std::shared_ptr<TiledGraph>;

       protected:
        ID id;
        MemoryLevel mem_level;
        std::string name;
        std::vector<std::shared_ptr<TiledNode>> nodes;
        std::vector<std::shared_ptr<TiledEdge>> in_edges;
        std::vector<std::shared_ptr<TiledEdge>> out_edges;
        std::vector<std::shared_ptr<TiledEdge>> intra_edges;
    };
};  // namespace tiledkernel::graph