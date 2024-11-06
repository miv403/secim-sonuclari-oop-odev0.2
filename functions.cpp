#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include "class.h"
#include <iomanip>

#define MAX_LINE 1024
#define REG_MAX 4

Elections::Elections(int regMax, CandNode* head)
    : regMax{regMax}, candHead {head}{ //member init list

}

Elections::~Elections() {
    for(int i = 0; i < candMax; ++i){
        delete[] votesByRegion[i];
    }
    delete[] votesByRegion;
}

void Elections::initMatrix() {

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

    // put votes to votesByRegion matrix

    CandNode* cand = candHead;

    if(cand == nullptr)
        return;

    for(int i = 0; i < candMax && cand != nullptr; ++i) {
        for(int j = 0; j < regMax && cand != nullptr; ++j) {
            if(cand->name == candidatesName[i] && cand != nullptr){
                votesByRegion[i][cand->reg - 1] = cand->votes;
                cand = cand->next;
            }
        }
    }
}

void Elections::calcRes() {


    for(int i = 0; i < candMax; ++i) {
        totalVotes.push_back(0);
    }

    for(int i = 0 ; i < candMax; ++i){
        for(int j = 0; j < regMax; ++j) {
            totalVotes.at(i) += votesByRegion[i][j];
        }
    }

    winner = 0;
    for(int i = 0; i < candMax; ++i){
        if(totalVotes[i] > totalVotes[winner])
            winner = i;
    }
    total = 0;
    for(auto i: totalVotes){
        total += i;
    }

}

void Elections::print() {
    cout << setw(36) << setfill('-') << right << "Election Results"
    << setw(20) << setfill('-') << " " << endl;
    cout << "Candidate" << setw(22) << setfill(' ')<< right << "Votes" << endl;

    cout << setw(9) << left << "Name";
    cout << setw(9) << right << "Region1";
    cout << setw(9) << "Region2";
    cout << setw(9) << "Region3";
    cout << setw(9) << "Region4";
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

    newNode->next = prev->next;
    prev->next = newNode;
}

void CandList::print() {

    if(empty()) return;

    CandNode* cursor = head;
    while(cursor != nullptr) {
        cout << cursor->name << " "
        << cursor->reg << " " << cursor->votes << endl;

        cursor = cursor->next;
    }
    

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

void CandList::readVotes(ifstream& file) {

    string line{};

    while (getline(file, line)) {
        vector<string> tokens = parseLine(line);

        string name = tokens.at(0);
        int reg = stoi(tokens.at(1));
        int votes = stoi(tokens.at(2));

        insert(name, reg, votes);
    }

}

void Elections::readCandidates(ifstream& file) {

    string line{};

    while(getline(file, line)){
        candidatesName.push_back(line);
    }
    candMax = candidatesName.size();
    sortCand();
}

void Elections::sortCand() {

    for (int i = 0; i < candMax - 1; i++) {
        for (int j = 0; j < candMax - i - 1; j++) {
            if (candidatesName.at(j) > candidatesName.at(j + 1)) {
                string tmp = candidatesName[j];
                candidatesName[j] = candidatesName[j + 1];
                candidatesName[j + 1] = tmp;
            }
        }
    }
}

int CandList::regMax() const{
    
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
