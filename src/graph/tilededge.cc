#include "graph/tilededge.hpp"
#include "graph/tilednode.hpp"
#include "error_handler.hpp"

namespace tiledkernel::graph {

    TiledEdge::TiledEdge(std::string name, std::shared_ptr<TiledNode> producer,
                         std::shared_ptr<TiledNode> consumer)
        : name(name), producer(producer), consumer(consumer) {
        id = ID::make();
    }

    void TiledEdge::inferType() {
        auto producer = getProducer();
        auto consumer = getConsumer();
        auto producer_type = producer->getType();
        auto consumer_type = consumer->getType();
        if (producer_type == NodeType::Buffer &&
            consumer_type == NodeType::Buffer) {
            if (producer->getMemLevel() >= consumer->getMemLevel()) {
                edge_type = EdgeType::Load;
            } else {
                edge_type = EdgeType::Store;
            }
        } else if (producer_type == NodeType::Buffer &&
                   consumer_type == NodeType::Operator) {
            edge_type = EdgeType::Compute;
        } else if (producer_type == NodeType::Operator &&
                   consumer_type == NodeType::Buffer) {
            edge_type = EdgeType::Compute;
        } else {
            TODO("Not implemented yet");
        }
    }
};  // namespace tiledkernel::graph