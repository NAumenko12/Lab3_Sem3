#include "test_support.h"
#include "struct/ochered/ochered1.h"

TEST(Queue, FirstInFirstOutAndReuse) {
    Queue1 queue;
    queue.Q1Insert("a");
    queue.Q1Insert("b");
    EXPECT_EQ(queue.Q1Front(), "a");
    EXPECT_EQ(queue.Q1Back(), "b");
    Output output;
    queue.Q1PrintQueue();
    EXPECT_EQ(output.Text(), "a\nb\n");
    queue.Q1PrintAddresses();
    queue.Q1Delete();
    EXPECT_EQ(queue.Q1Front(), "b");
    queue.Q1Delete();
    queue.Q1Insert("c");
    EXPECT_EQ(queue.Q1Front(), "c");
    EXPECT_EQ(queue.Q1Back(), "c");
    queue.Q1InitQueue();
    queue.Q1Delete();
    EXPECT_EQ(queue.Q1Front(), "");
    EXPECT_EQ(queue.Q1Back(), "");
}

TEST(Queue, CopiesRebuildTheirOwnTail) {
    Queue1 original;
    original.Q1Insert("a");
    original.Q1Insert("b");
    Queue1 copy(original), assigned;
    assigned.Q1Insert("old");
    assigned = original;
    copy.Q1Insert("c");
    assigned.Q1Delete();
    EXPECT_EQ(original.Q1Back(), "b");
    EXPECT_EQ(copy.Q1Back(), "c");
    EXPECT_EQ(assigned.Q1Front(), "b");
    EXPECT_EQ(original.Q1Front(), "a");
    Queue1& same = copy;
    copy = same;
    EXPECT_EQ(copy.Q1Back(), "c");
    Queue1 empty, emptyCopy(empty);
    assigned = emptyCopy;
    assigned.Q1Insert("new");
    EXPECT_EQ(assigned.Q1Back(), "new");
}

TEST(Queue, LoadedQueueCanAppendAndRejectsBrokenFile) {
    TestFile file;
    Queue1 queue, loaded;
    queue.Q1Insert("a");
    queue.Q1Insert("b");
    queue.Save(file.name);
    loaded.Load(file.name);
    loaded.Q1Insert("c");
    EXPECT_EQ(loaded.Q1Front(), "a");
    EXPECT_EQ(loaded.Q1Back(), "c");
    file.Write("bad");
    EXPECT_THROW(loaded.Load(file.name), runtime_error);
    loaded.Q1Insert("d");
    EXPECT_EQ(loaded.Q1Back(), "d");
    queue.Q1InitQueue();
    queue.Save(file.name);
    loaded.Load(file.name);
    loaded.Q1Insert("again");
    EXPECT_EQ(loaded.Q1Front(), "again");
}
