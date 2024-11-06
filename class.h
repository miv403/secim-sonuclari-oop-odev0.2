#include <vector>
#include <string>
#include <fstream>

#define REGION 4
using namespace std;


class CandNode {

public:
    string name;
    int reg;
    int votes;

    CandNode * next;
};

class Elections {

public:
    Elections(int , CandNode*); // dynamic alloc matrix
    ~Elections();  // dealloc votesByRegion
    void readCandidates(ifstream&);
    void sortCand();
    void initMatrix();
    void calcRes();
    void print();
private:
    int winner;
    int total;
    CandNode* candHead;
    int regMax;
    int candMax;
    vector<string> candidatesName; // candidate name vector
    vector<int> totalVotes; // total votes for each candidate vector
    int ** votesByRegion; // dynamic 2D array

};

class CandList {

public:
    
    CandNode* head;

    CandList();
    ~CandList();

    bool empty() const; // empty = true

    void insert(const string&, const int&, const int&);
        // insert ordered to list
    void remove();
        // remove from front of the list

    void readVotes(ifstream&);
    vector<string> parseLine(string&);

    int regMax() const;

    void print();

};