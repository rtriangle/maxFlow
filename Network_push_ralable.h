#ifndef Network_push_relable_h
#define Network_push_relable_h

#include <vector>

class Edge {
public:
	long long capacity, flow;
	size_t to;
	bool reverse;
	size_t reversedEdge;
	Edge(size_t to1, long long capacity1) : to(to1), capacity(capacity1), reverse(false), reversedEdge(0), flow(0) {}
	Edge(size_t to1, long long capacity1, size_t reversedEdge1) : to(to1), capacity(capacity1), reverse(false), reversedEdge(reversedEdge1), flow(0) {}
};

class Network {
private:
	std::vector< std::vector<Edge> > edge;
	std::vector< size_t > current;
	std::vector< long long > excess;
	std::vector< size_t > height;
	std::vector< size_t > vertices;
	size_t numberOfEdges, numberOfVertices;
	size_t start, finish;

public:
	Network(size_t n, size_t start1, size_t finish1);

	Network(){}

	void addEdge(size_t from1, size_t to1, long long capacity1);

	long long getEdgeFlow(size_t from, size_t to);

	size_t getStart();

	size_t getFinish();

	size_t getEdgeToOnPosition(size_t v, size_t pos);

	size_t getVertexDegree(size_t v);

	long long getCap(size_t v, size_t number);

	long long getFlow(size_t v, size_t number);

	size_t getNumberOfVertices();

	size_t getNumberOfEdges();

	void changeFlow(size_t u, size_t pos, long long addflow);

	Edge getEdgeOnPosition(size_t u, size_t pos);
};

#endif