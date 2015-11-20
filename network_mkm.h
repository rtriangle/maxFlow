#ifndef network_mkm_h
#define network_mkm_h

#include <vector>

class Edge {
public:
	long long capacity, flow;
	size_t to, reversedEdgeNumber;
	bool reverse;
	Edge(size_t _to, long long _capacity, size_t _reversedEdgeNumber, bool _reverse) :
		to(_to), capacity(_capacity), reversedEdgeNumber(_reversedEdgeNumber), reverse(_reverse) {}
};

class Network {
private:
	size_t numberOfEdges, numberOfVertices;
	size_t start, finish;
	std::vector< std::vector<Edge> > edge;
public:
	Network() {}
	Network(size_t n, size_t _start, size_t _finish) {
		edge.resize(n + 1);
		numberOfVertices = n;
		numberOfEdges = 0;
		start = _start;
		finish = _finish;
	}

	void addEdge(size_t _from, size_t _to, long long _capacity) {
		edge[_from].push_back(Edge(_to, _capacity, edge[_to].size(), false));
		edge[_to].push_back(Edge(_from, 0, edge[_from].size() - 1, true));
		edge[_from][edge[_from].size() - 1].flow = 0;
		edge[_to][edge[_to].size() - 1].flow = 0;
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
};

#endif