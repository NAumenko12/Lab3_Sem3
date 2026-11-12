#include "test_support.h"
#include "struct/stack/stack.h"

TEST(Stack, LastInFirstOut) {
    Stack stack;
    EXPECT_TRUE(stack.SIsEmptyStack());
    stack.SPush("first");
    stack.SPush("second");
    EXPECT_EQ(stack.STop(), "second");
    Output output;
    stack.SPrint(stack.GetHead());
    EXPECT_EQ(output.Text(), "second\nfirst\n");
    stack.SPrintAddresses();
    stack.SPop();
    EXPECT_EQ(stack.STop(), "first");
    stack.SPop();
    EXPECT_TRUE(stack.SIsEmptyStack());
    stack.SPop();
    EXPECT_EQ(stack.STop(), "");
}

TEST(Stack, CopyAndAssignmentKeepOwnElements) {
    Stack original;
    original.SPush("a");
    original.SPush("b");
    Stack copy(original), assigned;
    assigned = original;
    original.SPop();
    EXPECT_EQ(copy.STop(), "b");
    EXPECT_EQ(assigned.STop(), "b");
    EXPECT_NE(copy.GetHead(), assigned.GetHead());
    copy.SInitStack();
    EXPECT_TRUE(copy.SIsEmptyStack());
    EXPECT_FALSE(assigned.SIsEmptyStack());
}

TEST(Stack, BinaryLoadPreservesTopAndOldDataOnError) {
    TestFile file;
    Stack stack, loaded;
    stack.SPush("a");
    stack.SPush("b");
    stack.Save(file.name);
    loaded.SPush("old");
    loaded.Load(file.name);
    EXPECT_EQ(loaded.STop(), "b");
    loaded.SPop();
    EXPECT_EQ(loaded.STop(), "a");
    file.Write("bad");
    EXPECT_THROW(loaded.Load(file.name), runtime_error);
    EXPECT_EQ(loaded.STop(), "a");
    EXPECT_THROW(loaded.Load(file.name + "/absent"), runtime_error);
}
