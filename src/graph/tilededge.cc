#include "graph/tilededge.hpp"
#include "graph/tilednode.hpp"

namespace tiledkernel::graph {
    TiledEdge::TiledEdge() : producer(nullptr), consumer(nullptr) {
        id = ID::make();
    }

    TiledEdge::TiledEdge(std::shared_ptr<TiledNode> producer,
                         std::shared_ptr<TiledNode> consumer)
        : producer(producer), consumer(consumer) {
        id = ID::make();
    }
};  // namespace tiledkernel::graph