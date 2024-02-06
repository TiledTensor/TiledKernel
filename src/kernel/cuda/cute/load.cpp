#include "kernel/cuda/cute/load.hpp"

namespace tiledkernel::kernel::cuda::cute {
    // TODO: Implement this function.
    std::string load(BufferPtr a, BufferPtr b) {
        if (a->mem_level == MemoryLevel::Shared &&
            b->mem_level == MemoryLevel::RF) {
            // declare shared memory to register file copy plan.
            // tcu's wmma instruction prescribes a strict data to thread
            // mapping, in the current implementation, the shm-2-reg copy plan
            // is related to mma.

            /**
             * Examples:
             * typename KeTraits::TiledMma mma;
             * auto rA = make_s2rA(sA_ptr, tid, typename
             * KeTraits::SmemLayoutA{}, mma); auto rB = make_s2rB(sB_ptr, tid,
             * typename KeTraits::SmemLayoutB{}, mma);
             *
             * for (int i = 0; i < rA.get_iters(); ++i) {
             *      rA.copy(i);  // load A register tile from shared memory
             *      rB.copy(i);  // load B register tile from shared memory
             * }
             */
        }
    }
}  // namespace tiledkernel::kernel::cuda::cute