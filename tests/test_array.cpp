#include "test_support.h"
#include "struct/Array/massiv.h"

TEST(Array, InsertReplaceAndErase) {
    Array array(1);
    array.MAddEnd("a");
    array.MAddEnd("c");
    array.MAddIndex("b", 1);
    EXPECT_EQ(array.MLength(), 3);
    EXPECT_EQ(array.MGet(1), "b");
    array.MReplace("x", 1);
    array.MDeleteIndex(0);
    EXPECT_EQ(array.MGet(0), "x");
    EXPECT_EQ(array.MGet(1), "c");
    Output output;
    array.MPrint();
    EXPECT_EQ(output.Text(), "x c \n");
    output.Clear();
    array.MPrintAddresses();
    EXPECT_NE(output.Text().find("size = 2"), string::npos);
}

TEST(Array, IndependentCopiesAndReset) {
    Array original;
    original.MAddEnd("a");
    Array copy(original), assigned;
    assigned = original;
    original.MReplace("b", 0);
    EXPECT_EQ(copy.MGet(0), "a");
    EXPECT_EQ(assigned.MGet(0), "a");
    Array& same = original;
    original = same;
    EXPECT_EQ(original.MGet(0), "b");
    copy.MDestroy();
    Array empty(copy);
    assigned = empty;
    EXPECT_EQ(assigned.MLength(), 0);
    copy.MAddEnd("again");
    EXPECT_EQ(copy.MGet(0), "again");
    copy.MInitArray(0);
    EXPECT_EQ(copy.MLength(), 0);
}

TEST(Array, InvalidIndicesLeaveValuesUnchanged) {
    Array array;
    array.MAddEnd("a");
    Output output;
    array.MAddIndex("x", -1);
    array.MAddIndex("x", 5);
    array.MDeleteIndex(2);
    array.MReplace("x", -1);
    EXPECT_EQ(array.MGet(9), "");
    EXPECT_EQ(array.MLength(), 1);
    EXPECT_EQ(array.MGet(0), "a");
}

TEST(Array, BinaryRoundTripAndBrokenFile) {
    TestFile file;
    Array array(5), loaded;
    array.MAddEnd("Тест\nСтрока");
    array.MAddEnd(string("a\0b", 3));
    array.MAddEnd("");
    Serializable& object = array;
    object.Save(file.name);
    loaded.Load(file.name);
    EXPECT_EQ(loaded.MLength(), 3);
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(loaded.MGet(i), array.MGet(i));
    file.Write("bad");
    EXPECT_THROW(loaded.Load(file.name), runtime_error);
    EXPECT_EQ(loaded.MLength(), 3);
    EXPECT_THROW(array.Save(file.name + "/bad"), runtime_error);
}
