#include "test_support.h"
#include "struct/Array/massiv.h"
#include <cstring>
#include <climits>

TEST(ArrayEdges, BothSidesOfIndexBoundsPreserveContents) {
    Array array(1);
    array.MAddEnd("a");
    array.MAddIndex("b", 1);
    Output output;
    array.MDeleteIndex(-1);
    array.MReplace("bad", 2);
    EXPECT_EQ(array.MGet(-1), "");
    EXPECT_EQ(array.MLength(), 2);
    EXPECT_EQ(array.MGet(0), "a");
    EXPECT_EQ(array.MGet(1), "b");
}

TEST(ArrayEdges, ImpossibleCountsAndCapacityAreRejected) {
    Array array;
    array.MAddEnd("saved");
    TestFile file;
    array.Save(file.name);
    string good = file.Read();
    string badCount = good, badCapacity = good;
    size_t count = 3;
    size_t huge = INT_MAX;
    ++huge;
    memcpy(&badCount[0], &count, sizeof(count));
    memcpy(&badCapacity[sizeof(size_t)], &huge, sizeof(huge));
    for (const string& bad : {badCount, badCapacity}) {
        file.Write(bad);
        EXPECT_THROW(array.Load(file.name), runtime_error);
        EXPECT_EQ(array.MLength(), 1);
        EXPECT_EQ(array.MGet(0), "saved");
    }
    array.MDestroy();
    array.Save(file.name);
    Array loaded;
    loaded.Load(file.name);
    EXPECT_EQ(loaded.MLength(), 0);
    loaded.MAddEnd("again");
    EXPECT_EQ(loaded.MGet(0), "again");
}
