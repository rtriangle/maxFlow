#ifndef MKMÀlgo_h
#define MKMÀlgo_h

#include <vector>
#include <queue>
#include <list>
#include "Network.h"
#include "BFSforAll.h"

using namespace std;

bool EdgeChecker(long long capacity, long long flow) {
	return capacity - flow > 0;
}

class MKMAlgo {
private:
	std::vector <bool> deletedVerteces;
	std::vector <long long> inPotential, outPotential, potential;
	std::vector <size_t> currentEdge, level;
	static const long long INF = 1e16;

	void updatePotential(size_t v);

	void calculateVertexPotential(size_t u);

	void BFS(size_t u);

	long long pushByEdge(size_t u, size_t number, long long flowToPush);

	void sendFlow(size_t v, long long sizeFlow, bool toStart);

	void makeFlowThrough(size_t v);

	void deleteVertex(size_t u);

	void resizePotentials();

public:
	Network networkGraph;

	vector < pair <size_t, size_t> > inputEdgesOrder;

	~MKMAlgo() {}

	MKMAlgo(vector < pair <size_t, size_t> > &inputEdgesOrderIN);

	void findMaxFlow(Network networkGraphInput);

	void output(std::vector < std::pair <size_t, size_t> > inputEdgesOrder);

};

ostream& operator << (ostream& stream, MKMAlgo algo);

#endif