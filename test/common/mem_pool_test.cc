#include "mem_pool.h"

#include <gtest/gtest.h>

namespace Common {
namespace {
// A simple struct to test with the memory pool.
struct FooObj {
  int a = 0;
  int b = 0;
};

TEST(MemPoolTest, FullMemPoolReturnsNullptr) {
  size_t test_size = 3;
  MemPool<FooObj> pool(test_size);
  for (int i = 0; i < test_size; i++) {
    FooObj* obj = pool.allocate(FooObj{.a = i, .b = i});
    ASSERT_NE(obj, nullptr);
    ASSERT_EQ(obj->a, i);
  }
  ASSERT_EQ(pool.allocate(FooObj{1, 1}), nullptr);
}

TEST(MemPoolTest, DeallocationSuccess) {
  MemPool<FooObj> pool(1);
  FooObj* to_dealloc = nullptr;
  to_dealloc = pool.allocate(FooObj{1, 1});

  // Succeed alloc
  ASSERT_NE(to_dealloc, nullptr);
  // Fail next alloc
  ASSERT_EQ(pool.allocate(FooObj{1, 1}), nullptr);
  // Free space
  pool.deallocate(to_dealloc);
  to_dealloc = nullptr;
  to_dealloc = pool.allocate(FooObj{1, 1});
  ASSERT_NE(to_dealloc, nullptr);
}

}  // namespace
}  // namespace Common
