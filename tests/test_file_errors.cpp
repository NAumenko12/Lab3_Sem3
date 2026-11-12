#include "test_support.h"
#include "struct/Array/massiv.h"
#include "struct/hash/hesh_table.h"
#include "struct/hash/cuckoo_table.h"
#include "struct/tree/black_red_tree.h"

void CheckTruncatedFile(Serializable& object) {
    TestFile file;
    object.Save(file.name);
    string good = file.Read();
    for (size_t length = 0; length < good.size(); ++length) {
        SCOPED_TRACE(length);
        file.Write(good.substr(0, length));
        EXPECT_THROW(object.Load(file.name), runtime_error);
        object.Save(file.name);
        EXPECT_EQ(file.Read(), good);
    }
    file.Write(good + "extra");
    EXPECT_THROW(object.Load(file.name), runtime_error);
    object.Save(file.name);
    EXPECT_EQ(file.Read(), good);
    EXPECT_THROW(object.Load(file.name + "/missing"), runtime_error);
}

TEST(FileErrors, EveryTruncatedPrefixIsRejectedWithoutChangingData) {
    Array array;
    array.MAddEnd("value");
    array.MAddEnd("");
    FList first;
    first.FAddEnd("value");
    first.FAddEnd("");
    LList second;
    second.LAddEnd("value");
    second.LAddEnd("");
    HashTable hash;
    hash.HInsert("key", "value");
    Tree tree;
    tree.Add(5);
    tree.Add(2);
    tree.Add(8);
    CuckooTable cuckoo;
    bool inserted;
    cuckoo.CInsert("key", inserted);
    ASSERT_TRUE(inserted);
    for (Serializable* object :
         vector<Serializable*>{&array, &first, &second, &hash, &tree, &cuckoo}) {
        CheckTruncatedFile(*object);
    }
}
