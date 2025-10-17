#include <gtest/gtest.h>
#include "lf_queue.h"

namespace Common
{
    namespace
    {
        // A simple struct to test with the memory pool.
        struct FooObj
        {
            int a = 0;
            int b = 0;
        };

        TEST(LfQueueTest, QueueExpectationsMet)
        {
            size_t test_size{3};
            LfQueue<FooObj> q(test_size);
            for (int i = 0; i < test_size; i++)
            {
                FooObj *obj_w = q.getNextWriteTo();
                ASSERT_NE(obj_w, nullptr);
                q.updateNextWrite();
                obj_w->a = i;
                ASSERT_EQ(q.size(), i + 1);
            }
            ASSERT_EQ(q.getNextWriteTo(), nullptr);

            for (int i = 0; i < test_size; i++)
            {
                const FooObj *obj_r = q.getNextRead();
                ASSERT_NE(obj_r, nullptr);
                ASSERT_EQ(obj_r->a, i);
                q.updateNextRead();
                ASSERT_EQ(q.size(), (test_size - (i + 1)));
            }
        }

    } // namespace
} // namespace Common
