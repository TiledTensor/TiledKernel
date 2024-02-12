#pragma once
#include "mem_level.hpp"
#include "microop.hpp"
#include "platform.hpp"
#include "access_map.hpp"
#include "id.hpp"
#include <vector>
#include <string>

namespace tiledkernel::graph {

    class TiledNode;

    class TiledEdge {
       public:
        TiledEdge();
        TiledEdge(std::shared_ptr<TiledNode> producer,
                  std::shared_ptr<TiledNode> consumer);

        std::shared_ptr<TiledNode> getProducer() { return producer; }

        std::shared_ptr<TiledNode> getConsumer() { return consumer; }

        void setProducer(std::shared_ptr<TiledNode> producer) {
            this->producer = producer;
        }

        void setConsumer(std::shared_ptr<TiledNode> consumer) {
            this->consumer = consumer;
        }

        using Pointer = std::shared_ptr<TiledEdge>;

       protected:
        // std::shared_ptr<TiledBuffer> input;
        // std::shared_ptr<TiledBuffer> output;
        ID id;
        std::shared_ptr<TiledNode> producer;
        std::shared_ptr<TiledNode> consumer;
        // std::shared_ptr<AccessMap> access_map;
    };

    using EdgePtr = std::shared_ptr<TiledEdge>;
};  // namespace tiledkernel::graph