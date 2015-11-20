#ifndef network_push_relable_h
#define network_push_relable_algo_h

#include <vector>

class Edge {
public:
	long long capacity, flow;
	size_t to;
	bool reverse;
	size_t reversedEdge;
	Edge(size_t _to, long long _capacity) : to(_to), capacity(_capacity), reverse(false), reversedEdge(0), flow(0) {}
	Edge(size_t _to, long long _capacity, size_t _reversedEdge) : to(_to), capacity(_capacity), reverse(false), reversedEdge(_reversedEdge), flow(0) {}
};

class Network {
private:
	std::vector< std::vector<Edge> > edge;
	std::vector< size_t > current;
	std::vector< long long > excess;
	std::vector< size_t > height;
	std::vector< size_t > vertexes;
	size_t numberOfEdges, numberOfVertices;
	size_t start, finish;
public:
	Network(size_t n, size_t _start, size_t _finish) {
		edge.resize(n + 1);
		numberOfVertices = n;
		numberOfEdges = 0;
		start = _start;
		finish = _finish;
	}

	Network(){}

	void addEdge(size_t _from, size_t _to, long long _capacity) {
		edge[_from].push_back(Edge(_to, _capacity, edge[_to].size()));
		edge[_to].push_back(Edge(_from, 0, edge[_from].size() - 1));
		if (_from == start) {
			edge[_from][edge[_from].size() - 1].flow = (_from == start ? _capacity : 0);
			edge[_to][edge[_to].size() - 1].flow = (_from == start ? -_capacity : 0);
		}
		++numberOfEdges;
	}

	long long getEdgeFlow(size_t from, size_t to) {
		return edge[from][to].flow;
	}

	size_t getStart() {
		return start;
	}

	size_t getFinish() {
		return finish;
	}

	size_t getEdgeToOnPosition(size_t v, size_t pos) {
		if (pos < edge[v].size())
			return edge[v][pos].to;
		else
			return 0;
	}

	size_t getVertexDegree(size_t v) {
		return edge[v].size();
	}

	long long getCap(size_t v, size_t number) {
		return edge[v][number].capacity;
	}

	long long getFlow(size_t v, size_t number) {
		return edge[v][number].flow;
	}

	size_t getNumberOfVertices() {
		return numberOfVertices;
	}

	size_t getNumberOfEdges() {
		return numberOfEdges;
	}

	void changeFlow(size_t u, size_t pos, long long addflow) {
		edge[u][pos].flow += addflow;
		size_t tmp = edge[u][pos].reversedEdge;
		edge[edge[u][pos].to][tmp].flow -= addflow;
	}
};

#endif