#include "test_support.h"
#include "struct/hash/hesh_table.h"

TEST(Hash, CollisionsUpdateAndRemoval) {
    HashTable table(32);
    for (string key : {"Aa", "BB", "C#"})
        table.HInsert(key, key);
    table.HInsert("Aa", "updated");
    EXPECT_EQ(table.HLength(), 3);
    bool found;
    string value;
    table.HGet("Aa", value, found);
    EXPECT_TRUE(found);
    EXPECT_EQ(value, "updated");
    table.HDelete("BB");
    table.HDelete("C#");
    table.HDelete("absent");
    table.HContains("Aa", found);
    EXPECT_TRUE(found);
    table.HContains("BB", found);
    EXPECT_FALSE(found);
    EXPECT_EQ(table.HLength(), 1);
    Output output;
    table.HPrint();
    EXPECT_EQ(output.Text(), "Aa -> updated\n");
    table.HPrintAddresses();
    EXPECT_NE(output.Text().find("size = 1"), string::npos);
}

TEST(Hash, GrowthAndIndependentCopies) {
    HashTable original(2);
    for (int i = 0; i < 100; ++i)
        original.HInsert(to_string(i), "value");
    HashTable copy(original), assigned;
    assigned = original;
    original.HClear();
    EXPECT_EQ(original.HLength(), 0);
    EXPECT_EQ(copy.HLength(), 100);
    for (int i = 0; i < 100; ++i) {
        string value;
        bool found;
        assigned.HGet(to_string(i), value, found);
        EXPECT_TRUE(found);
        EXPECT_EQ(value, "value");
    }
    HashTable& same = copy;
    copy = same;
    EXPECT_EQ(copy.HLength(), 100);
    original.HDestroy();
    original.HDelete("x");
    bool found;
    original.HContains("x", found);
    EXPECT_FALSE(found);
    HashTable empty(original);
    EXPECT_EQ(empty.HLength(), 0);
    original.HInsert("again", "ok");
    EXPECT_EQ(original.HLength(), 1);
}

TEST(Hash, BinaryRoundTripAndErrors) {
    TestFile file;
    HashTable table(32), loaded;
    table.HInsert("Aa", "Тест");
    table.HInsert("BB", "");
    table.Save(file.name);
    loaded.Load(file.name);
    EXPECT_EQ(loaded.HLength(), 2);
    bool found;
    string value;
    loaded.HGet("Aa", value, found);
    EXPECT_TRUE(found);
    EXPECT_EQ(value, "Тест");
    file.Write(file.Read().substr(0, 20));
    EXPECT_THROW(loaded.Load(file.name), runtime_error);
    loaded.HGet("Aa", value, found);
    EXPECT_EQ(value, "Тест");
    EXPECT_THROW(table.Save(file.name + "/bad"), runtime_error);
    table.HDestroy();
    table.Save(file.name);
    loaded.Load(file.name);
    EXPECT_EQ(loaded.HLength(), 0);
}

TEST(Hash, StrategiesCanBeClonedAndReplaced) {
    PolynomialHash polynomial;
    CuckooHash1 first;
    CuckooHash2 second;
    for (HashFunction* function : vector<HashFunction*>{&polynomial, &first, &second}) {
        auto copy = function->Clone();
        EXPECT_EQ(copy->HHash("abc", 11), function->HHash("abc", 11));
        EXPECT_GE(copy->HHash("abc", 11), 0);
        EXPECT_LT(copy->HHash("abc", 11), 11);
    }
    HashTable table(0, nullptr);
    table.HInsert("a", "b");
    EXPECT_EQ(table.HLength(), 1);
    HashTable alternate(8, make_unique<CuckooHash1>());
    alternate.HInsert("key", "value");
    bool found;
    alternate.HContains("key", found);
    EXPECT_TRUE(found);
}
