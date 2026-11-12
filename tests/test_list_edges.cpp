#include "test_support.h"

TEST(ListEdges, FListRejectsInvalidPositionsAndForeignTargets) {
    FList list;
    Output output;
    list.FDeleteValue("missing");
    list.FAddIndex("a", 0);
    list.FAddEnd("b");
    ListNode foreign("outside");
    list.FAddBefore(&foreign, "x");
    list.FDeleteBefore(&foreign);
    list.FDeleteBefore(list.GetHead());
    list.FDeleteBefore(nullptr);
    list.FDeleteAfter(list.GetHead()->next);
    list.FAddIndex("x", -1);
    list.FDeleteIndex(1);
    list.FDeleteIndex(1);
    list.FDeleteIndex(20);
    EXPECT_EQ(Values(list), (vector<string>{"a"}));
    list.FDeleteIndex(0);
    EXPECT_EQ(list.GetHead(), nullptr);
}

TEST(ListEdges, FListTraversesToTheLastAndPreviousNodes) {
    FList list;
    for (string value : {"a", "b", "c", "d"})
        list.FAddEnd(value);
    list.FDeleteBefore(list.GetHead()->next->next->next);
    EXPECT_EQ(Values(list), (vector<string>{"a", "b", "d"}));
    list.FDeleteEnd();
    list.FDeleteValue("missing");
    EXPECT_EQ(Values(list), (vector<string>{"a", "b"}));
    list.FDeleteValue("a");
    EXPECT_EQ(Values(list), (vector<string>{"b"}));
    list.FDeleteIndex(0);
    EXPECT_TRUE(Values(list).empty());
}

TEST(ListEdges, LListRepairsTailWhenDeletingAfterNode) {
    LList list;
    list.LAddEnd("a");
    list.LAddEnd("b");
    list.LDeleteAfter(list.GetHead());
    EXPECT_EQ(list.GetHead(), list.GetTail());
    EXPECT_EQ(Values(list), (vector<string>{"a"}));
    Output output;
    list.LDeleteAfter(list.GetTail());
    list.LDeleteBefore(list.GetHead());
    list.LDeleteIndex(-1);
    list.LDeleteIndex(8);
    list.LDeleteValue("missing");
    EXPECT_EQ(Values(list), (vector<string>{"a"}));
    list.LAddEnd("b");
    list.LAddEnd("c");
    list.LDeleteValue("c");
    EXPECT_EQ(Values(list), (vector<string>{"a", "b"}));
    EXPECT_EQ(list.GetTail()->next, nullptr);
}
