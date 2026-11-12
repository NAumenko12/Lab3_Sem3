#include "test_support.h"

TEST(LList, InsertionsKeepBothDirections) {
    LList list;
    list.LAddEnd("b");
    list.LAddFirst("a");
    list.LAddAfter(list.GetTail(), "d");
    list.LAddBefore(list.GetTail(), "c");
    list.LAddBefore(list.GetHead(), "start");
    list.LAddIndex("end", 5);
    EXPECT_EQ(Values(list), (vector<string>{"start", "a", "b", "c", "d", "end"}));
    Output output;
    list.LPrintBac(list.GetTail());
    EXPECT_EQ(output.Text(), "end\nd\nc\nb\na\nstart\n");
    output.Clear();
    list.LPrint(list.GetHead());
    EXPECT_EQ(output.Text(), "start\na\nb\nc\nd\nend\n");
    list.LFind(list.GetHead(), "a");
    list.LFind(list.GetHead(), "missing");
    list.LPrintAddresses(list.GetHead());
    EXPECT_NE(output.Text().find("missing не найден"), string::npos);
}

TEST(LList, DeletionsRepairHeadTailAndLinks) {
    LList list;
    for (string value : {"a", "b", "c", "d", "e"})
        list.LAddEnd(value);
    list.LDeleteAfter(list.GetHead());
    list.LDeleteBefore(list.GetTail());
    EXPECT_EQ(Values(list), (vector<string>{"a", "c", "e"}));
    list.LDeleteNode(list.GetHead()->next);
    list.LDeleteValue("a");
    list.LDeleteEnd();
    EXPECT_EQ(list.GetHead(), nullptr);
    EXPECT_EQ(list.GetTail(), nullptr);
    list.LAddIndex("x", 0);
    list.LAddEnd("y");
    list.LDeleteIndex(1);
    list.LDeleteFirst();
    EXPECT_TRUE(Values(list).empty());
    list.LAddEnd("z");
    list.LInitList();
    EXPECT_EQ(list.GetTail(), nullptr);
}

TEST(LList, CopyAndAssignmentAreIndependent) {
    LList original;
    original.LAddEnd("a");
    original.LAddEnd("b");
    LList copy(original), assigned;
    assigned.LAddEnd("old");
    assigned = original;
    original.LDeleteEnd();
    EXPECT_EQ(Values(copy), (vector<string>{"a", "b"}));
    EXPECT_EQ(Values(assigned), Values(copy));
    EXPECT_NE(copy.GetTail(), assigned.GetTail());
    LList& same = copy;
    copy = same;
    EXPECT_EQ(Values(copy), Values(assigned));
}

TEST(LList, InvalidPositionsAndRoundTrip) {
    LList list;
    Output output;
    list.LDeleteFirst();
    list.LDeleteEnd();
    list.LDeleteNode(nullptr);
    list.LAddAfter(nullptr, "x");
    list.LAddBefore(nullptr, "x");
    list.LAddIndex("x", -1);
    list.LAddIndex("x", 5);
    list.LDeleteAfter(nullptr);
    list.LDeleteBefore(nullptr);
    list.LDeleteValue("absent");
    list.LAddEnd("Тест");
    list.LAddEnd("");
    TestFile file;
    list.Save(file.name);
    LList loaded;
    loaded.Load(file.name);
    EXPECT_EQ(Values(loaded), Values(list));
    file.Write(file.Read().substr(0, 9));
    EXPECT_THROW(loaded.Load(file.name), runtime_error);
    EXPECT_EQ(Values(loaded), Values(list));
    EXPECT_THROW(list.Save(file.name + "/bad"), runtime_error);
}
