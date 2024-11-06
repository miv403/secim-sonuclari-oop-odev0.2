#include <vector>
#include <string>

#define REGION 4
using namespace std;



class Elections {

public:
    Elections(int regMax, int candMax); // dynamic alloc matrix
    ~Elections();  // dealloc votesByRegion

private:
    int regMax;
    int candMax;
    vector<string> candidatesName;
    vector<int> totalVotes;
    int ** votesByRegion; // dynamic 2D array
};

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

    bool empty() const; // empty=true

    void insert(const string&, const int&, const int&);
        // insert ordered to list
    void remove();
        // remove from front of the list
};