#include "test_support.h"
#include "struct/Array/massiv.h"
#include "struct/hash/cuckoo_table.h"
#include "struct/hash/hesh_table.h"
#include "struct/ochered/ochered1.h"
#include "struct/stack/stack.h"
#include "struct/tree/black_red_tree.h"

TEST(JsonSerialization, RoundTripForAllStructures) {
    Array array(3), loadedArray;
    FList first, loadedFirst;
    LList second, loadedSecond;
    Stack stack, loadedStack;
    Queue1 queue, loadedQueue;
    HashTable hash, loadedHash;
    CuckooTable cuckoo, loadedCuckoo;
    Tree tree, loadedTree;

    string value = "Значение \"со скобками\"\\\n\t";
    value += '\0';
    array.MAddEnd(value);
    first.FAddEnd(value);
    second.LAddEnd("");
    stack.SPush(value);
    queue.Q1Insert(value);
    hash.HInsert("ключ", value);
    bool inserted;
    cuckoo.CInsert("a", inserted);
    ASSERT_TRUE(inserted);
    tree.Add(5);
    tree.Add(-2);
    tree.Add(8);

    vector<pair<Serializable*, Serializable*>> structures{
        {&array, &loadedArray},   {&first, &loadedFirst}, {&second, &loadedSecond},
        {&stack, &loadedStack},   {&queue, &loadedQueue}, {&hash, &loadedHash},
        {&cuckoo, &loadedCuckoo}, {&tree, &loadedTree},
    };
    for (const auto& pair : structures) {
        TestFile file;
        file.name += ".json";
        pair.first->Save(file.name);
        string saved = file.Read();
        ASSERT_EQ(saved.front(), '{');
        pair.second->Load(file.name);
        pair.second->Save(file.name);
        EXPECT_EQ(file.Read(), saved);
    }
    EXPECT_EQ(loadedStack.STop(), value);
}

TEST(JsonSerialization, RejectsMalformedJsonWithoutChangingData) {
    TestFile file;
    file.name += ".json";
    FList list;
    list.FAddEnd("сохранено");
    for (const char* bad : {
             R"({"type":"llist","data":[0]})",
             R"({"wrong":"flist","data":[0]})",
             R"({"type":"flist","wrong":[0]})",
             R"({"type":"flist","data":[-1]})",
             R"({"type":"flist","data":[01,"a"]})",
             R"({"type":"flist","data":[18446744073709551616]})",
             R"({"type":"flist","data":[9999999999999999999999]})",
             R"({"type":"flist","data":[0,]})",
             R"({"type":"flist","data":[1,"unfinished)",
             R"({"type":"flist","data":[1,"\q"]})",
             R"({"type":"flist","data":[1,"\u000g"]})",
             R"({"type":"flist","data":[1,"\ud83d"]})",
             R"({"type":"flist","data":[1,"\ud83d\u0123"]})",
             R"({"type":"flist","data":[1,"\ude00"]})",
             R"({"type":"flist","data":[0]}x)",
         }) {
        SCOPED_TRACE(bad);
        file.Write(bad);
        EXPECT_THROW(list.Load(file.name), runtime_error);
        EXPECT_EQ(Values(list), vector<string>{"сохранено"});
    }
    file.Write(R"({"type":"flist","data":[1,"\\\/\"\b\f\n\r\t\u0000\u00e9\u20AC\uD83D\uDE00"]})");
    list.Load(file.name);
    string expected = "\\/\"\b\f\n\r\t";
    expected += '\0';
    expected += "é€😀";
    EXPECT_EQ(Values(list), vector<string>{expected});

    Tree tree;
    file.Write(R"({"type":"tree","data":[1,2147483648,0,0,0]})");
    EXPECT_THROW(tree.Load(file.name), runtime_error);
}
