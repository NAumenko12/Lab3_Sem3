#include "test_support.h"
#include "struct/tree/black_red_tree.h"
#include <cstring>

string TreeData(int value, char color, const string& left = "0", const string& right = "0") {
    char bytes[sizeof(int)];
    memcpy(bytes, &value, sizeof(value));
    return "1" + string(bytes, sizeof(bytes)) + color + left + right;
}

TEST(TreeErrors, InvalidOrderingColorsAndBlackHeightAreRejected) {
    Tree tree;
    tree.Add(10);
    TestFile file;
    tree.Save(file.name);
    string good = file.Read();
    vector<string> invalid{
        TreeData(10, '0', TreeData(20, '0'), TreeData(15, '0')),
        TreeData(10, '0', TreeData(5, '0'), TreeData(3, '0')),
        TreeData(10, '0', TreeData(5, '1', TreeData(3, '1')), TreeData(15, '0')),
        TreeData(10, '0', TreeData(5, '1', "0", TreeData(7, '1')), TreeData(15, '0')),
        TreeData(10, '0', TreeData(5, '0')),
        TreeData(10, 'x')};
    for (const string& bad : invalid) {
        file.Write(bad);
        EXPECT_THROW(tree.Load(file.name), runtime_error);
        tree.Save(file.name);
        EXPECT_EQ(file.Read(), good);
    }
}
