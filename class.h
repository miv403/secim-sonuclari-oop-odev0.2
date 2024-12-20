#include <fstream>
#include <vector>
#include <string>

using namespace std;

class CandNode {

public:
    string name;
    int reg;
    int votes;

    CandNode * next;
};

class CandList {

public:
    CandNode* head;

    CandList();
    ~CandList();

    bool empty() const; // empty = true

    void insert(const string&, const int&, const int&); // insert ordered
                                                        // to list
    void remove(); // remove from front of the list
    #ifdef DEBUG
    void print() const; // for debug purposes, prints all list.
    #endif

    void readVotes(ifstream&); //reads vote.dat and inserts them to the list
    vector<string> parseLine(string&); // parses lines with whitespace

    int regMax() const;

};

class Elections {

    public:
        Elections(int, CandNode*);  // dynamic alloc matrix
        ~Elections();  // dealloc votesByRegion

        void readCandidates(ifstream&);
        void initMatrix();
        void calcRes();
        void print();

    private:
        void sortCand();

        CandNode* candHead;
        int winner;
        int total;
        int regMax;
        int candMax;

        int ** votesByRegion; // dynamic 2D array
        string * candidatesName;
        int * totalVotes;

};
