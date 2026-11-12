#include "test_support.h"
#include "struct/hash/hesh_table.h"
#include <cstring>
#include <climits>

TEST(HashErrors, DuplicateKeysAndInvalidSizesKeepOriginalTable) {
    HashTable table(8);
    table.HInsert("key", "saved");
    TestFile file;
    table.Save(file.name);
    string good = file.Read();
    string duplicate = good + good.substr(2 * sizeof(size_t));
    size_t count = 2;
    memcpy(&duplicate[0], &count, sizeof(count));
    string badCount = good, badCapacity = good;
    count = 9;
    size_t huge = INT_MAX;
    ++huge;
    memcpy(&badCount[0], &count, sizeof(count));
    memcpy(&badCapacity[sizeof(size_t)], &huge, sizeof(huge));
    for (const string& bad : {duplicate, badCount, badCapacity}) {
        file.Write(bad);
        EXPECT_THROW(table.Load(file.name), runtime_error);
        bool found;
        string value;
        table.HGet("key", value, found);
        EXPECT_TRUE(found);
        EXPECT_EQ(value, "saved");
        EXPECT_EQ(table.HLength(), 1);
    }
}
