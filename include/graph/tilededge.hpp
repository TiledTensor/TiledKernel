#pragma once
#include "mem_level.hpp"
#include "platform.hpp"
#include "access_map.hpp"
#include "id.hpp"
#include <vector>
#include <string>

namespace tiledkernel::graph {

    enum class EdgeType { Load, Store, Compute };

    class TiledNode;

    class TiledEdge {
       public:
        TiledEdge(std::string name = "",
                  std::shared_ptr<TiledNode> producer = nullptr,
                  std::shared_ptr<TiledNode> consumer = nullptr);

        std::string getName() { return name; }

        ID getID() { return id; }

        std::shared_ptr<TiledNode> getProducer() { return producer; }

        std::shared_ptr<TiledNode> getConsumer() { return consumer; }

        void setProducer(std::shared_ptr<TiledNode> producer) {
            this->producer = producer;
        }

        void setConsumer(std::shared_ptr<TiledNode> consumer) {
            this->consumer = consumer;
        }

        void setAccessMapI(AccessMap::Pointer access_map_i) {
            this->access_map_i = access_map_i;
        }

        void setAccessMapO(AccessMap::Pointer access_map_o) {
            this->access_map_o = access_map_o;
        }

        AccessMap::Pointer getAccessMapI() { return access_map_i; }

        AccessMap::Pointer getAccessMapO() { return access_map_o; }

        void inferType();

        using Pointer = std::shared_ptr<TiledEdge>;

       protected:
        std::string name;
        ID id;
        EdgeType edge_type;
        std::shared_ptr<TiledNode> producer;
        std::shared_ptr<TiledNode> consumer;
        AccessMap::Pointer access_map_i;
        AccessMap::Pointer access_map_o;
    };

    using EdgePtr = std::shared_ptr<TiledEdge>;
};  // namespace tiledkernel::graph