#include "test_support.h"
#include "struct/hash/cuckoo_table.h"

TEST(Cuckoo, DisplacementDuplicateAndDelete) {
    CuckooTable table;
    bool inserted, found;
    for (string key : {"a", "l", "w"}) {
        table.CInsert(key, inserted);
        EXPECT_TRUE(inserted);
    }
    table.CInsert("l", inserted);
    EXPECT_TRUE(inserted);
    for (string key : {"a", "l", "w"}) {
        table.CContains(key, found);
        EXPECT_TRUE(found);
    }
    table.CDelete("l");
    table.CDelete("absent");
    table.CContains("l", found);
    EXPECT_FALSE(found);
    table.CContains("a", found);
    EXPECT_TRUE(found);
    Output output;
    table.CPrint();
    EXPECT_NE(output.Text().find("Индекс | первая таблица"), string::npos);
}

TEST(Cuckoo, CopiesAndFailedInsertionPreserveKeys) {
    CuckooTable table;
    vector<string> accepted;
    bool inserted, found, rejected = false;
    for (int i = 0; i < 100; ++i) {
        string key = to_string(i);
        table.CInsert(key, inserted);
        if (!inserted) {
            rejected = true;
            table.CContains(key, found);
            EXPECT_FALSE(found);
            break;
        }
        accepted.push_back(key);
    }
    EXPECT_TRUE(rejected);
    for (const string& key : accepted) {
        table.CContains(key, found);
        EXPECT_TRUE(found);
    }
    ASSERT_FALSE(accepted.empty());
    CuckooTable copy(table), assigned;
    assigned = table;
    table.CDelete(accepted[0]);
    copy.CContains(accepted[0], found);
    EXPECT_TRUE(found);
    assigned.CContains(accepted[0], found);
    EXPECT_TRUE(found);
    CuckooTable& same = copy;
    copy = same;
    copy.CContains(accepted[0], found);
    EXPECT_TRUE(found);
}

TEST(Cuckoo, BinaryRoundTripAndBrokenFile) {
    TestFile file;
    CuckooTable table(nullptr, nullptr), loaded;
    bool inserted, found;
    for (string key : {"a", "l", "w"})
        table.CInsert(key, inserted);
    table.Save(file.name);
    loaded.Load(file.name);
    loaded.Save(file.name);
    string good = file.Read();
    for (string key : {"a", "l", "w"}) {
        loaded.CContains(key, found);
        EXPECT_TRUE(found);
    }
    for (string bad : {good.substr(0, 3), good + "x", string("z")}) {
        file.Write(bad);
        EXPECT_THROW(loaded.Load(file.name), runtime_error);
        loaded.CContains("a", found);
        EXPECT_TRUE(found);
    }
    EXPECT_THROW(table.Save(file.name + "/bad"), runtime_error);
}
