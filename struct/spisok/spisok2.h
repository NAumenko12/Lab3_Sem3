#pragma once
#include "../serialization/serializable.h"
#include "../Node/double_list.h"

class LList : public Serializable {
private:
    LNode* head;
    LNode* tail;

public:
    LList();
    LList(const LList& other);
    LList& operator=(const LList& other);
    ~LList() override;

    void LInitList();

    void LPrint(LNode* flist) const;
    void LPrintBac(LNode* last) const;

    void LAddAfter(LNode* flist, const string& chislo);
    void LAddFirst(const string& chislo);
    void LAddEnd(const string& chislo);
    void LAddBefore(LNode* flist, const string& chislo);
    void LAddIndex(const string& chislo, int index);

    void LDeleteFirst();
    void LDeleteEnd();
    void LDeleteAfter(LNode* flist);
    void LDeleteBefore(LNode* flist);
    void LDeleteIndex(int index);
    void LDeleteNode(LNode* flist);
    void LDeleteValue(const string& chislo);

    void LFind(LNode* flist, const string& chislo) const;
    void LPrintAddresses(LNode* first) const;

    LNode* GetHead() const;
    LNode* GetTail() const;
    void Save(const string& filename) const override;
    void Load(const string& filename) override;
};
