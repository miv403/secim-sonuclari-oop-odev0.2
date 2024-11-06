#include "functions.cpp"
#include <fstream>

#define VOTE "./dat/vote.dat"
#define CAND "./dat/cand.dat"

int main() {


    CandList candList;

    ifstream voteFile;
    voteFile.open(VOTE);

    
    candList.readVotes(voteFile);

#ifdef DEBUG
    candList.print();
#endif

    int regions = candList.regMax();

    Elections elect(regions, candList.head);

    ifstream candFile;
    candFile.open(CAND);
    elect.readCandidates(candFile);
    elect.initMatrix();
    elect.calcRes();
    elect.print();

    return 0;
}