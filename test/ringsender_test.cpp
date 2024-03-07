#include "gtest/gtest.h"

#include "ringsender.h"

TEST(RingsenderTest, RingSenderIsNull)
{
    struct ring_sender *rs = NULL;
    EXPECT_EQ(create_ring_sender(3000, 3001, 1000, rs), -1);
}

TEST(RingsenderTest, RingSenderIsNullForCancel)
{
    struct ring_sender *rs = NULL;
    EXPECT_EQ(cancel_ring_sender(rs), -1);
}
