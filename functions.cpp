#include <iostream>
#include "class.h"

Elections::Elections(int regMax, int candMax)
    : regMax{regMax}, candMax{candMax} { //member init list

    // dynamic votesByRegion matrix alloc

    votesByRegion = new int*[candMax];
    for(int i = 0; i < candMax; ++i) {
        votesByRegion[i] = new int [regMax];
    }
    for(int i = 0; i < candMax; ++i){
        for(int j = 0; j < regMax; ++j){
            votesByRegion[i][j] = 0;
        }
    }
}

Elections::~Elections() {
    for(int i = 0; i < candMax; ++i){
        delete[] votesByRegion[i];
    }
    delete[] votesByRegion;
}

CandList::CandList() {
    head = nullptr;
}

CandList::~CandList() {
    while(!empty()) remove();
}

bool CandList::empty() const {
    return head == nullptr;
}

void CandList::insert(const string& name,
                    const int& reg,
                    const int& votes) {
    
    CandNode* newNode = new CandNode;

    newNode->name = name;
    newNode->reg = reg;
    newNode->votes= votes;

    if(empty()) {
        head = newNode;
        newNode->next = nullptr;
        return;
    }

    // inserting newNode to head of the list
    if(newNode->name < head->name) {
        newNode->next = head;
        head = newNode;
        return;
    }

    CandNode* prev= head;

    while (prev->next != nullptr)
    {
        if(newNode->name >= prev->next->name)
            prev = prev->next;
        else
            break;
    }
    // inserting newNode after prev

    newNode->next	= prev->next;
}

void CandList::remove() {
    if (empty())
    {
        cout << "List is empty !" << endl;
        return;
    }

    CandNode* tmp = head;  // save current head
    head = head->next;       // skip over old head
    delete tmp;             // delete the old head
}