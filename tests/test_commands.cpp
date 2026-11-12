#include "test_support.h"
#include "ProcessCommand.h"

TEST(Commands, AllStructuresPersistBetweenQueries) {
    TestFile file;
    Output output;
    vector<vector<string>> sequences{
        {"FInitList", "FAddEnd a", "FAddFirst b", "FAddIndex c 1", "FAddBefore 0 x",
         "FAddAfter 0 y", "FPrintBack", "FPrintAddresses", "FFind a", "FDeleteBefore 1",
         "FDeleteAfter 0", "FDeleteIndex 1", "FDeleteFirst", "FDeleteEnd", "FAddEnd z",
         "FDeleteValue z", "FPrint"},
        {"LInitList", "LAddEnd a", "LAddFirst b", "LAddIndex c 1", "LAddBefore 0 x",
         "LAddAfter 0 y", "LPrintBac", "LPrintAddresses", "LFind a", "LDeleteBefore 1",
         "LDeleteAfter 0", "LDeleteIndex 1", "LDeleteFirst", "LDeleteEnd", "LAddEnd z",
         "LDeleteValue z", "LAddEnd q", "LDeleteNode 0", "LPrint"},
        {"SInitStack", "SPush a", "SPush b", "SPrint", "SPrintAddresses", "SIsEmptyStack", "SPop",
         "STop"},
        {"Q1InitQueue", "Q1Insert a", "Q1Insert b", "Q1PrintQueue", "Q1PrintAddresses", "Q1Front",
         "Q1Delete", "Q1Back"},
        {"MInitArray 2", "MAddEnd a", "MAddIndex b 0", "MReplace c 1", "MPrint", "MPrintAddresses",
         "MResize", "MLength", "MDeleteIndex 0", "MGet 0", "MDestroy", "MAddEnd x", "MGet 0"},
        {"TInitTree", "TAdd 5", "TAdd 2", "TPrint", "TPrintPryam", "TPrintAddresses", "TFind 2",
         "TDelete 5", "TDelete 2", "TPrint"},
        {"HInitTable 8", "HInsert key value", "HGet key", "HGet absent", "HLength", "HPrint",
         "HPrintAddresses", "HContains key", "HDelete key", "HClear", "HDestroy",
         "HInsert other data", "HGet other"},
        {"CInitTable", "CInsert a", "CInsert l", "CInsert w", "CPrint", "CDelete l",
         "CContains l"}};
    vector<string> expected{"", "", "a\n", "b\n", "x\n", "\n", "data\n", "false\n"};
    for (size_t i = 0; i < sequences.size(); ++i) {
        for (const string& query : sequences[i]) {
            output.Clear();
            ASSERT_EQ(ProcessCommand(file.name, query), 0) << query;
        }
        EXPECT_EQ(output.Text(), expected[i]);
    }
}

TEST(Commands, FailedQueriesKeepTheFileUnchanged) {
    TestFile file;
    vector<vector<string>> cases{
        {"FInitList", "FAddEnd", "FAddEnd x extra", "FAddIndex x -1", "FAddAfter 0 x", "FBad"},
        {"LInitList", "LAddBefore -1 x", "LDeleteNode 4", "LBad"},
        {"SInitStack", "SPop", "STop", "SPush", "SBad"},
        {"Q1InitQueue", "Q1Insert", "Q1Front extra", "Q1Bad"},
        {"MInitArray 2", "MGet -1", "MGet 1x", "MReplace a 0", "MInitArray 0", "MBad"},
        {"TInitTree", "TAdd 2147483648", "TAdd", "TBad"},
        {"HInitTable 8", "HInsert a", "HInitTable 0", "HBad"},
        {"CInitTable", "CInsert", "CContains a extra", "CBad"}};
    for (const auto& queries : cases) {
        ASSERT_EQ(ProcessCommand(file.name, queries[0]), 0);
        string before = file.Read();
        for (size_t i = 1; i < queries.size(); ++i) {
            EXPECT_THROW(ProcessCommand(file.name, queries[i]), exception) << queries[i];
            EXPECT_EQ(file.Read(), before);
        }
    }
    EXPECT_THROW(ProcessCommand(file.name, " "), invalid_argument);
    EXPECT_THROW(ProcessCommand(file.name, "unknown"), invalid_argument);
    file.Write("broken");
    EXPECT_THROW(ProcessCommand(file.name, "SPush x"), runtime_error);
    EXPECT_EQ(file.Read(), "broken");
}

TEST(Commands, HelpShowsHowToRunAndSelectStructures) {
    Output output;
    PrintHelp();
    EXPECT_NE(output.Text().find("--file"), string::npos);
    EXPECT_NE(output.Text().find("--query"), string::npos);
    EXPECT_NE(output.Text().find("FInitList"), string::npos);
    EXPECT_NE(output.Text().find("CInitTable"), string::npos);
}
