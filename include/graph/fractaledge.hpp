#include "mem_level.hpp"

#include <set>
#include <string>

namespace fractalkernel::graph {
    enum EdgeType {
        Input,
        Output
    };


    template<class Input, class Output>
    class FractalEdge {
        public:
            FractalEdge(std::set<Input> inputs, std::set<Output> outputs, MemoryLevel mem_level, EdgeType edge_type) {
                this->inputs = inputs;
                this->outputs = outputs;
                this->mem_level = mem_level;
                this->edge_type = edge_type;
            }
    
            std::set<Input> getInputs() {
                return inputs;
            }
    
            std::set<Output> getOutputs() {
                return outputs;
            }
    
            MemoryLevel getMmemLevel() {
                return mem_level;
            }
    
            EdgeType getEdgeType() {
                return edge_type;
            }

            std::string map();

           protected:
            std::set<Input> inputs;
            std::set<Output> outputs;
            MemoryLevel mem_level;
            EdgeType edge_type;
        };
};  // namespace fractalkernel::graph