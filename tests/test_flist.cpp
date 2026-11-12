#include "test_support.h"

TEST(FList, InsertionsKeepOrder) {
    FList list;
    list.FAddEnd("b");
    list.FAddFirst("a");
    list.FAddAfter(list.GetHead()->next, "d");
    list.FAddBefore(list.GetHead()->next->next, "c");
    list.FAddBefore(list.GetHead(), "start");
    list.FAddIndex("end", 5);
    EXPECT_EQ(Values(list), (vector<string>{"start", "a", "b", "c", "d", "end"}));
    Output output;
    list.FPrintBack(list.GetHead());
    EXPECT_EQ(output.Text(), "end\nd\nc\nb\na\nstart\n");
    output.Clear();
    list.FPrint(list.GetHead());
    EXPECT_EQ(output.Text(), "start\na\nb\nc\nd\nend\n");
    output.Clear();
    list.FFind(list.GetHead(), "c");
    list.FFind(list.GetHead(), "missing");
    list.FPrintAddresses(list.GetHead());
    EXPECT_NE(output.Text().find("missing не найден"), string::npos);
}

TEST(FList, RemovalsAndReuse) {
    FList list;
    for (string value : {"a", "b", "c", "d", "e"})
        list.FAddEnd(value);
    list.FDeleteBefore(list.GetHead()->next);
    list.FDeleteAfter(list.GetHead());
    list.FDeleteValue("e");
    EXPECT_EQ(Values(list), (vector<string>{"b", "d"}));
    list.FAddIndex("c", 1);
    list.FDeleteIndex(1);
    list.FDeleteEnd();
    list.FDeleteFirst();
    EXPECT_EQ(list.GetHead(), nullptr);
    list.FAddEnd("x");
    list.FInitList();
    EXPECT_TRUE(Values(list).empty());
}

TEST(FList, CopiesOwnTheirNodes) {
    FList original;
    original.FAddEnd("a");
    original.FAddEnd("b");
    FList copy(original), assigned;
    assigned.FAddFirst("old");
    assigned = original;
    original.FDeleteFirst();
    EXPECT_EQ(Values(copy), (vector<string>{"a", "b"}));
    EXPECT_EQ(Values(assigned), Values(copy));
    EXPECT_NE(copy.GetHead(), assigned.GetHead());
    FList& same = copy;
    copy = same;
    EXPECT_EQ(Values(copy), Values(assigned));
}

TEST(FList, InvalidPositionsAndBinaryFiles) {
    FList list;
    Output output;
    list.FDeleteFirst();
    list.FDeleteEnd();
    list.FDeleteIndex(-1);
    list.FAddAfter(nullptr, "x");
    list.FAddBefore(nullptr, "x");
    list.FAddIndex("x", 5);
    list.FDeleteBefore(nullptr);
    list.FDeleteAfter(nullptr);
    list.FAddFirst("a");
    list.FAddEnd("b");
    list.FAddEnd("c");
    list.FDeleteBefore(list.GetHead()->next->next);
    EXPECT_EQ(Values(list), (vector<string>{"a", "c"}));
    TestFile file;
    list.Save(file.name);
    FList loaded;
    loaded.Load(file.name);
    EXPECT_EQ(Values(loaded), Values(list));
    file.Write(file.Read().substr(0, 9));
    EXPECT_THROW(loaded.Load(file.name), runtime_error);
    EXPECT_EQ(Values(loaded), Values(list));
    EXPECT_THROW(list.Save(file.name + "/bad"), runtime_error);
}
