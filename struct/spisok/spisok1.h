#pragma once
#include "../Node/list_node.h"
#include "../serialization/serializable.h"

class FList : public Serializable {
private:
    ListNode* head;

public:
    FList();
    FList(const FList& other);
    FList& operator=(const FList& other);
    ~FList() override;

    void FInitList();

    void FAddAfter(ListNode* flist, const string& chislo);
    void FAddFirst(const string& chislo);
    void FAddEnd(const string& chislo);
    void FAddBefore(ListNode* target, const string& chislo);
    void FAddIndex(const string& chislo, int index);

    void FDeleteAfter(ListNode* flist);
    void FDeleteFirst();
    void FDeleteEnd();
    void FDeleteBefore(ListNode* target);
    void FDeleteIndex(int index);
    void FDeleteValue(const string& chislo);

    void FPrintBack(ListNode* flist) const;
    void FPrint(ListNode* flist) const;
    void FPrintAddresses(ListNode* flist) const;
    void FFind(ListNode* flist, const string& chislo) const;

    ListNode* GetHead() const;

    void Save(const string& filename) const override;
    void Load(const string& filename) override;
};