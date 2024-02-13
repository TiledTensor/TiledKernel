#include "graph/tiledgraph.hpp"
#include "graph/tilednode.hpp"
#include "graph/tilededge.hpp"
#include "error_handler.hpp"
#include <unordered_map>
#include <iostream>

namespace tiledkernel::graph {
    TiledGraph::TiledGraph(MemoryLevel mem_level, std::string name,
                           std::vector<std::shared_ptr<TiledNode>> nodes,
                           std::vector<std::shared_ptr<TiledEdge>> in_edges,
                           std::vector<std::shared_ptr<TiledEdge>> out_edges,
                           std::vector<std::shared_ptr<TiledEdge>> intra_edges)
        : mem_level(mem_level),
          name(name),
          nodes(nodes),
          in_edges(in_edges),
          out_edges(out_edges),
          intra_edges(intra_edges) {
        id = ID::make();
        ctx = std::make_shared<TiledContext>();

        // Connect nodes and edges
        connect();
    }

    std::vector<std::shared_ptr<TiledNode>> TiledGraph::topoSort() {
        std::vector<NodePtr> sorted_nodes;
        std::unordered_map<NodePtr, int32_t> in_degrees;

        for (auto node : nodes) {
            in_degrees[node] = node->in_degrees;
        }

        while (!in_degrees.empty()) {
            for (auto node = in_degrees.begin(); node != in_degrees.end();
                 node++) {
                if (node->second == 0) {
                    std::cout << "Node: " << node->first->id << std::endl;
                    sorted_nodes.push_back(node->first);
                    for (auto successor : (node->first)->successors) {
                        in_degrees[successor] -= 1;
                        std::cout << "Successor: " << successor->id
                                  << std::endl;
                    }
                    in_degrees.erase(node);
                    break;
                }
            }
        }

        return sorted_nodes;
    }

    void TiledGraph::connect() {
        for (auto node : nodes) {
            for (auto edge : node->in_edges) {
                edge->setConsumer(node);
            }
            for (auto edge : node->out_edges) {
                edge->setProducer(node);
            }
        }

        for (auto edge : intra_edges) {
            edge->getProducer()->successors.push_back(edge->getConsumer());
            edge->getConsumer()->predecessors.push_back(edge->getProducer());
        }

        for (auto in_edge : in_edges) {
            in_edge->getConsumer()->in_degrees -= 1;
        }
    }
}  // namespace tiledkernel::graph