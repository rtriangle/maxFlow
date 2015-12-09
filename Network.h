#ifndef Network_mkm_h
#define Network_mkm_h

#include <vector>
#include "FlowEdge.h"
#include "Graph.h"

class Network:public Graph {

private:
	size_t numberOfEdges, numberOfVertices, start, finish;
	std::vector< size_t > current;
	std::vector< std::vector<FlowEdge> > edge;
	std::vector< long long > excess;
	std::vector< size_t > height;

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

	void changeFlow(size_t u, size_t pos, long long addflow);

	FlowEdge getEdgeOnPosition(size_t u, size_t pos);
};

#endif