#ifndef Network_mkm_h
#define Network_mkm_h

#include <vector>

class Edge {
public:
	long long capacity, flow;
	size_t to, reversedEdgeNumber;
	bool reverse;
	Edge(size_t to1, long long capacity1, size_t reversedEdgeNumber1, bool reverse1) :
		to(to1), capacity(capacity1), reversedEdgeNumber(reversedEdgeNumber1), reverse(reverse1) {}
};

class Network {
private:
	size_t numberOfEdges, numberOfVertices;
	size_t start, finish;
	std::vector< std::vector<Edge> > edge;
public:
	Network() {}
	Network(size_t n, size_t start1, size_t finish1);

	void addEdge(size_t from, size_t to, long long capacity);

	size_t getNumberOfVertices();

	size_t getNumberOfEdges();

	size_t getStart();

	size_t getFinish();

	size_t getEdgeToOnPosition(size_t v, size_t pos);

	size_t getVertexDegree(size_t v);

	size_t getReversedEdgeNumber(size_t u, size_t number);

	long long getCap(size_t v, size_t number);

	void setCap(size_t v, size_t number, long long _capacity);

	//v->v[number] returns cap(v[number] -> v)
	long long getCapReversedEdge(size_t v, size_t number);

	long long getFlowReversedEdge(size_t v, size_t number);

	long long getFlow(size_t v, size_t number);

	void setFlow(size_t v, size_t number, long long _flow);

	bool getEdgeReversed(size_t v, size_t number);

	Edge getEdgeOnPosition(size_t u, size_t pos);
};

#endif