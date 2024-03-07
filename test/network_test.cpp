#include "gtest/gtest.h"

#include "network.h"

TEST(NetworkTest, RingSocketIsNull)
{
    struct ring_socket *rs = NULL;
    EXPECT_EQ(create_ring_socket(3000, rs), -1);
}