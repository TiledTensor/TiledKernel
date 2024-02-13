#include "graph/tilededge.hpp"
#include "graph/tilednode.hpp"

namespace tiledkernel::graph {

    TiledEdge::TiledEdge(std::string name, std::shared_ptr<TiledNode> producer,
                         std::shared_ptr<TiledNode> consumer)
        : name(name), producer(producer), consumer(consumer) {
        id = ID::make();
    }
};  // namespace tiledkernel::graph