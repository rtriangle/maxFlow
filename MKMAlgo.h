#ifndef MKM_algo_h
#define MKM_algo_h

#include <vector>
#include <queue>
#include <list>
#include "Network_mkm.h"

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

ostream& operator << (ostream& stream, MKMAlgo algo) {
	long long resFlow = 0;
	for (int i = 0; i < algo.networkGraph.getVertexDegree(1); ++i)
		if (algo.networkGraph.getEdgeToOnPosition(1, i) != 1) {
			resFlow += algo.networkGraph.getFlow(1, i);
		}
	cout << resFlow << '\n';
	for (int i = 0; i < algo.inputEdgesOrder.size(); ++i) {
		if (algo.inputEdgesOrder[i].first != -1)
			cout << algo.networkGraph.getFlow(algo.inputEdgesOrder[i].first, algo.inputEdgesOrder[i].second) << "\n";
		else
			cout << "0\n";
	}
	return stream;
}

#endif