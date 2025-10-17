#pragma once

#include <iostream>
#include <vector>
#include <atomic>
#include <memory>

#include "macros.h"

namespace Common {
    template<typename T>
    class MemPool final {
      public:
        explicit MemPool(size_t pool_size) :
        store_(pool_size, {T(), true}) {
            ASSERT(pool_size > 0, "Must allocate at least one block.");
            // Required for deallocate, to find ObjBlock idx.
            ASSERT(reinterpret_cast<const ObjBlock*>(
                &(store_[0].obj_) == &(store_[0].obj_)),
                "Object should be first member of ObjBlock."
            );
        }

        MemPool() = delete;
        MemPool(const MemPool&) = delete;
        MemPool(const MemPool&&) = delete;
        MemPool& operator=(const MemPool&) = delete;
        MemPool& operator=(const MemPool&&) = delete;

        template<typename... Args>
        // Allocate next free memory block in pool and place the object.
        // TODO: segmentation fault happens here, reconsider using a
        // a vector of ObjBlock.
        T* allocate(Args... args) noexcept {
            if (is_full_) [[unlikely]] {
                return nullptr;
            }
            ObjBlock* obj_block = &(store_[next_free_]);
            ASSERT(obj_block->is_free_,
                "Expected next free ObjBlock to be free.");
            // Address of next free obj block
            T* ret = &(obj_block->obj_);
            // Place new obj in block
            ret = std::construct_at(ret, T(args...));
            // Update is_free_ and next free index.
            obj_block->is_free_ = false;
            updateNextFree();
            return ret;
        }

        // Deallocate the 'obj' memory by setting is_free_ to true.
        void deallocate(T* obj) noexcept {
            const auto obj_idx = (reinterpret_cast<ObjBlock*>(obj) - &store_[0]);
            ASSERT(obj_idx >= 0 && static_cast<size_t>(obj_idx) < store_.size(),
                "Object being deallocated does not belong to this MemPool.");
            ASSERT(!store_[obj_idx].is_free_, "Expected object block to be in-use.");

            store_[obj_idx].is_free_ = true;
            if (is_full_) [[unlikely]] {
                is_full_ = false;
            }
        }
      private:
        struct ObjBlock {
            T obj_;
            bool is_free_{true};
        };
        std::vector<ObjBlock> store_;
        size_t next_free_;
        bool is_full_ = false;

        // Update the next free index. Exit fails when MemPool is full.
        void updateNextFree() noexcept {
            const size_t init_free = next_free_;
            while(!store_[next_free_].is_free_) {
                next_free_++;
                if (next_free_ == store_.size()) [[unlikely]] {
                    next_free_ = 0;
                }
                if (next_free_ == init_free) [[unlikely]] {
                    is_full_ = true;
                    return;
                }
            }
        }
    };
}  // namespace Common
