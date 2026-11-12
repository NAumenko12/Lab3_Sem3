#include "test_support.h"
#include "struct/tree/black_red_tree.h"
#include <random>
#include <set>

TEST(Tree, SearchCopyAndClear) {
    Tree tree;
    for (int value : {8, 3, 12, 1, 6, 10, 14, 4, 7})
        tree.Add(value);
    tree.Add(8);
    Tree copy(tree), assigned;
    assigned.Add(100);
    assigned = tree;
    tree.Delete(8);
    Output output;
    copy.Find(copy.GetRoot(), 8);
    EXPECT_EQ(output.Text().find("8 найден"), 0u);
    output.Clear();
    tree.Find(tree.GetRoot(), 8);
    EXPECT_EQ(output.Text(), "8 не найден\n");
    output.Clear();
    copy.PrintPryam(copy.GetRoot());
    EXPECT_NE(output.Text().find("8"), string::npos);
    copy.PrintAddresses(copy.GetRoot());
    EXPECT_NE(output.Text().find("Значение:"), string::npos);
    Tree& same = assigned;
    assigned = same;
    EXPECT_NE(assigned.GetRoot(), copy.GetRoot());
    copy.InitTree();
    EXPECT_EQ(copy.GetRoot(), nullptr);
    Tree empty(copy);
    EXPECT_EQ(empty.GetRoot(), nullptr);
}

TEST(Tree, MixedOperationsAgreeWithSetAndRemainBalanced) {
    Tree tree, loaded;
    TestFile file;
    set<int> expected;
    mt19937 random(16);
    Output output;
    for (int step = 0; step < 1200; ++step) {
        int value = random() % 80;
        if (random() % 2) {
            tree.Add(value);
            expected.insert(value);
        } else {
            tree.Delete(value);
            expected.erase(value);
        }
        if (step % 40 == 0) {
            tree.Save(file.name);
            ASSERT_NO_THROW(loaded.Load(file.name));
            for (int key = 0; key < 80; ++key) {
                output.Clear();
                loaded.Find(loaded.GetRoot(), key);
                string message = to_string(key) + (expected.count(key) ? " найден" : " не найден");
                EXPECT_EQ(output.Text().find(message), 0u);
            }
        }
    }
}

TEST(Tree, BinaryLoadRejectsInvalidTreeAndKeepsOriginal) {
    TestFile file;
    Tree tree, loaded;
    tree.Add(7);
    tree.Save(file.name);
    string good = file.Read();
    loaded.Load(file.name);
    loaded.Save(file.name);
    EXPECT_EQ(file.Read(), good);
    string redRoot = good;
    redRoot[1 + sizeof(int)] = '1';
    for (const string& bad : {good.substr(0, 2), good + "x", string("z"), redRoot}) {
        file.Write(bad);
        EXPECT_THROW(loaded.Load(file.name), runtime_error);
        Output output;
        loaded.Find(loaded.GetRoot(), 7);
        EXPECT_EQ(output.Text().find("7 найден"), 0u);
    }
    EXPECT_THROW(tree.Save(file.name + "/bad"), runtime_error);
    tree.InitTree();
    tree.Save(file.name);
    loaded.Load(file.name);
    EXPECT_EQ(loaded.GetRoot(), nullptr);
}
