#ifndef PushRelableAlgo_h
#define PushRelableAlgo_h

#include <vector>
#include <queue>
#include <list>
#include "Network.h"

using namespace std;

class PushRelabelAlgo {
private:
	std::vector <size_t> height, currentEdge;
	std::vector <long long> excess;
	size_t doIt;
	std::queue <size_t> q;
	std::list< size_t > verteces;
	const long long INF = 1e16;

	void initialize();

	void discharge(size_t v, Network& networkGraph);

	bool push(size_t _u, Network& networkGraph);

	void relabel(size_t _u, size_t degU, Network& networkGraph);

public:

	~PushRelabelAlgo() {}

	Network networkGraph;

	vector < pair <size_t, size_t> > inputEdgesOrder;

	void findMaxFlow(Network& networkGraph);

	void output(std::vector < std::pair <size_t, size_t> > inputEdgesOrder);
};

ostream& operator << (ostream& stream, PushRelabelAlgo algo);

#endif