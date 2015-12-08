#ifndef network_mkm_h
#define network_mkm_h

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
	Network(size_t n, size_t start1, size_t finish1) {
		edge.resize(n + 1);
		numberOfVertices = n;
		numberOfEdges = 0;
		start = start1;
		finish = finish1;
	}

	void addEdge(size_t from, size_t to, long long capacity) {
		edge[from].push_back(Edge(to, capacity, edge[to].size(), false));
		edge[to].push_back(Edge(from, 0, edge[from].size() - 1, true));
		edge[from][edge[from].size() - 1].flow = 0;
		edge[to][edge[to].size() - 1].flow = 0;
		++numberOfEdges;
	}

	size_t getNumberOfVertices() {
		return numberOfVertices;
	}

	size_t getNumberOfEdges() {
		return numberOfEdges;
	}

	size_t getStart() {
		return start;
	}
	size_t getFinish() {
		return finish;
	}

	size_t getEdgeToOnPosition(size_t v, size_t pos) {
		if (pos < edge[v].size()) {
			return edge[v][pos].to;
		}
		else
			return 0;
	}

	size_t getVertexDegree(size_t v) {
		return edge[v].size();
	}

	size_t getReversedEdgeNumber(size_t u, size_t number) {
		return edge[u][number].reversedEdgeNumber;
	}

	long long getCap(size_t v, size_t number) {
		return edge[v][number].capacity;
	}

	void setCap(size_t v, size_t number, long long _capacity) {
		edge[v][number].capacity = _capacity;
	}

	//v->v[number] returns cap(v[number] -> v)
	long long getCapReversedEdge(size_t v, size_t number) {
		size_t v1 = edge[v][number].to;
		size_t rNumber = edge[v][number].reversedEdgeNumber;
		return edge[v1][rNumber].capacity;
	}
	long long getFlowReversedEdge(size_t v, size_t number) {
		size_t v1 = edge[v][number].to;
		size_t rNumber = edge[v][number].reversedEdgeNumber;
		return edge[v1][rNumber].flow;
	}
	long long getFlow(size_t v, size_t number) {
		return edge[v][number].flow;
	}

	void setFlow(size_t v, size_t number, long long _flow) {
		edge[v][number].flow = _flow;
	}

	bool getEdgeReversed(size_t v, size_t number) {
		return edge[v][number].reverse;
	}

	Edge getEdgeOnPosition(size_t u, size_t pos) {
		return edge[u][pos];
	}
};

#endif