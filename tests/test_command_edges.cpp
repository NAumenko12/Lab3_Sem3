#include "test_support.h"
#include "ProcessCommand.h"

TEST(CommandEdges, ListBoundariesRejectQueriesWithoutSaving) {
    TestFile file;
    vector<vector<string>> cases{{"FInitList", "FAddIndex a 0", "FDeleteBefore 0", "FDeleteAfter 0",
                                  "FAddAfter -1 x", "FDeleteIndex 9"},
                                 {"LInitList", "LAddIndex a 0", "LDeleteBefore 0", "LDeleteAfter 0",
                                  "LAddIndex x -1", "LDeleteIndex 9"}};
    for (const auto& queries : cases) {
        ASSERT_EQ(ProcessCommand(file.name, queries[0]), 0);
        ASSERT_EQ(ProcessCommand(file.name, queries[1]), 0);
        string before = file.Read();
        for (size_t i = 2; i < queries.size(); ++i) {
            EXPECT_THROW(ProcessCommand(file.name, queries[i]), out_of_range) << queries[i];
            EXPECT_EQ(file.Read(), before);
        }
    }
}

TEST(CommandEdges, ArrayRejectsOutOfRangeIndicesAndMalformedArguments) {
    TestFile file;
    ASSERT_EQ(ProcessCommand(file.name, "MInitArray 2"), 0);
    ASSERT_EQ(ProcessCommand(file.name, "MAddEnd a"), 0);
    string before = file.Read();
    for (string query : {"MGet 1", "MAddIndex b 2", "MAddIndex b -1", "MReplace b 1",
                         "MDeleteIndex 1", "MAddEnd \"unfinished", "MGet \"\""}) {
        EXPECT_THROW(ProcessCommand(file.name, query), exception) << query;
        EXPECT_EQ(file.Read(), before);
    }
}

TEST(CommandEdges, CuckooCycleDoesNotOverwriteTheSavedTable) {
    TestFile file;
    ASSERT_EQ(ProcessCommand(file.name, "CInitTable"), 0);
    bool rejected = false;
    int added = 0;
    for (; added < 100; ++added) {
        string before = file.Read();
        try {
            ProcessCommand(file.name, "CInsert " + to_string(added));
        } catch (const runtime_error& error) {
            EXPECT_NE(string(error.what()).find("цикл"), string::npos);
            EXPECT_EQ(file.Read(), before);
            rejected = true;
            break;
        }
    }
    ASSERT_TRUE(rejected);
    EXPECT_GT(added, 0);
    for (int i = 0; i < added; ++i) {
        Output output;
        ASSERT_EQ(ProcessCommand(file.name, "CContains " + to_string(i)), 0);
        EXPECT_EQ(output.Text(), "true\n");
    }
}
