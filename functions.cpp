#include <cstddef>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

#include "class.h"

#define CAND "./dat/cand.dat"

Elections::Elections(int regMax, CandNode* head)
    : regMax{regMax}, candHead {head}{ //member init list
    
    ifstream cand;
    cand.open(CAND);
    string line;
    candMax = 0;
    while(getline(cand,line)) { candMax++; }

    candidatesName = new string[candMax];
    totalVotes = new int[candMax];

    for(int i = 0; i < candMax; ++i) {
        totalVotes[i] = 0;
    }
}

Elections::~Elections() {   // destructor method
                            // deallocs votesByRegion dynamic matrix
    delete [] candidatesName;
    delete [] totalVotes;

    for(int i = 0; i < candMax; ++i){
        delete[] votesByRegion[i];
    }
    delete[] votesByRegion;
}

void Elections::readCandidates(ifstream& file) {
    // reads cand.dat file and inserts the list
    // to candidatesName vector

    string line{};
    for(size_t i = 0; i < candMax; ++i) {
        getline(file, line);
        candidatesName[i] = line;
    }
    sortCand();
}

void Elections::sortCand() {
    // sorts the candidatesName vector alphabetically
    // using bubble sort
    for (int i = 0; i < candMax - 1; i++) {
        for (int j = 0; j < candMax - i - 1; j++) {
            if (candidatesName[j] > candidatesName[j + 1]) {
                string tmp = candidatesName[j];
                candidatesName[j] = candidatesName[j + 1];
                candidatesName[j + 1] = tmp;
            }
        }
    }
}

void Elections::initMatrix() {

    // dynamic votesByRegion matrix alloc

    votesByRegion = new int*[candMax];
    for(int i = 0; i < candMax; ++i) {
        votesByRegion[i] = new int [regMax];
    }

    // initializing votesByRegion matrix with zero

    for(int i = 0; i < candMax; ++i){
        for(int j = 0; j < regMax; ++j){
            votesByRegion[i][j] = 0;
        }
    }

    // puts vote values to votesByRegion matrix

    CandNode* cand = candHead; // to walk CandList linked list

    if(cand == nullptr) // list emptyness check
        return;
        // TODO this block might be optimized!
    for(int i = 0; i < candMax; ++i) { // TODO nullptr!
        for(int j = 0; j < regMax && cand != nullptr; ++j) {
            if(cand->name == candidatesName[i]){
                votesByRegion[i][cand->reg - 1] = cand->votes;
                cand = cand->next;
            }
        }
    }
}

void Elections::initVotes() { // used in Elections::calcRes()

    // initializing totalVotes vector with zero

}

void Elections::calcRes() { // calculate results

    initVotes(); // inits totalVotes vector w/0

    for(int i = 0 ; i < candMax; ++i){
        for(int j = 0; j < regMax; ++j) {
            totalVotes[i] += votesByRegion[i][j];
        }
    }

    // evaluating winning candidate's index
    winner = 0;
    for(int i = 1; i < candMax; ++i){
        if(totalVotes[i] > totalVotes[winner])
            winner = i;
    }

    // evaluating total number of votes
    total = 0;
    for(size_t i = 0; i < candMax; ++i) {
        total += totalVotes[i];
    }
}

void Elections::print() { 
    // printing the matrix tabular format
    // and printing results
    // this block is static.

    cout << setw(36) << setfill('-') << right << "Election Results"
    << setw(20) << setfill('-') << " " << endl;
    cout << "Candidate" << setw(22) << setfill(' ')<< right << "Votes" << endl;

    cout << setw(9) << left << "Name";

    // FIXME use regMax to proper output
    for(int i = 1; i <= regMax; ++i) {
        cout << setw(8) << right << "Region" << i;
    }

    cout << setw(9) << "Total" << endl;

    cout << setw(55) << setfill('-') << "" << endl;

    for(int i = 0; i < candMax; ++i) {
        cout << setw(9) << setfill(' ') << left << candidatesName[i];
        for(int j = 0; j < regMax; ++j){
            cout << setw(9) << right << votesByRegion[i][j];
        }
        cout << setw(9) << right << totalVotes[i] << endl;
    }
    cout << endl;
    cout << "Winner: " << candidatesName[winner] << ", ";
    cout << "Votes Received: " << totalVotes[winner] << endl;

    cout << "Total votes polled: " << total;
}

CandList::CandList() { // initiliazes linked list
    head = nullptr;
}

CandList::~CandList() { // deallocs the linked list
    while(!empty()) remove();
}

bool CandList::empty() const { // emptiness check
    return head == nullptr;
}

void CandList::insert(const string& name,
                    const int& reg,
                    const int& votes) {
    
    // inserting new node in alphabetic order

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

    newNode->next = prev->next;
    prev->next = newNode;
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

#ifdef DEBUG
void CandList::print() const { // for debug purposes

    if(empty()) return;

    CandNode* cursor = head;
    while(cursor != nullptr) {
        cout << cursor->name << " "
        << cursor->reg << " " << cursor->votes << endl;

        cursor = cursor->next;
    }
    

}
#endif

void CandList::readVotes(ifstream& file) {

    // reads the vote.dat file
    // and inserts the list to CandList linked list

    string line{};

    while (getline(file, line)) {
        vector<string> tokens = parseLine(line);

        string name = tokens.at(0);
        int reg = stoi(tokens.at(1));
        int votes = stoi(tokens.at(2));

        insert(name, reg, votes);
    }

}

vector<string> CandList::parseLine(string& line) {
    vector<string> tokens;

    // alınan satırın boşluk ile ayrılarak vektöre eklenmesi
    size_t pos = 0;
    string token;
    string delimiter = " ";

    while ((pos = line.find(delimiter)) != string::npos) {
        token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos + delimiter.length());
    }
    tokens.push_back(line);

    return tokens;
}

int CandList::regMax() const{
    
    // determining maximum region to init suitable matrix
    if(empty()) return 0;

    int regMax{};
    
    CandNode* cursor = head;
    while(cursor != nullptr) {
        if(cursor->reg > regMax) {
            regMax = cursor->reg;
        }

        cursor = cursor->next;
    }

    return regMax;
}
