#include "test_support.h"
#include "struct/hash/cuckoo_table.h"
#include <cstring>

TEST(CuckooEdges, MissingCollisionAndDeletionFromFirstTable) {
    CuckooTable table;
    bool inserted, found;
    table.CInsert("a", inserted);
    ASSERT_TRUE(inserted);
    table.CContains("l", found);
    EXPECT_FALSE(found);
    table.CInsert("l", inserted);
    ASSERT_TRUE(inserted);
    table.CContains("a", found);
    EXPECT_TRUE(found);
    table.CDelete("l");
    table.CContains("l", found);
    EXPECT_FALSE(found);
    table.CContains("a", found);
    EXPECT_TRUE(found);
}

TEST(CuckooEdges, WrongCellAndRepeatedKeyInFileAreRejected) {
    CuckooTable table;
    bool inserted;
    table.CInsert("a", inserted);
    ASSERT_TRUE(inserted);
    TestFile file;
    table.Save(file.name);
    string good = file.Read();
    CuckooHash1 first;
    CuckooHash2 second;
    int position = first.HHash("a", 11);
    int other = 11 + second.HHash("a", 11);
    for (int mode = 0; mode < 2; ++mode) {
        string bad;
        for (int cell = 0; cell < 22; ++cell) {
            bool used = mode == 0 ? cell == (position + 1) % 11 : cell == position || cell == other;
            bad += used ? '1' : '0';
            if (used) {
                size_t length = 1;
                char bytes[sizeof(size_t)];
                memcpy(bytes, &length, sizeof(length));
                bad.append(bytes, sizeof(bytes));
                bad += 'a';
            }
        }
        file.Write(bad);
        EXPECT_THROW(table.Load(file.name), runtime_error);
        table.Save(file.name);
        EXPECT_EQ(file.Read(), good);
    }
}
