#ifndef network_push_relable_h
#define network_push_relable_algo_h

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
	Network(size_t n, size_t start1, size_t finish1) {
		edge.resize(n + 1);
		numberOfVertices = n;
		numberOfEdges = 0;
		start = start1;
		finish = finish1;
	}

	Network(){}

	void addEdge(size_t from1, size_t to1, long long capacity1) {
		edge[from1].push_back(Edge(to1, capacity1, edge[to1].size()));
		edge[to1].push_back(Edge(from1, 0, edge[from1].size() - 1));
		if (from1 == start) {
			edge[from1][edge[from1].size() - 1].flow = (from1 == start ? capacity1 : 0);
			edge[to1][edge[to1].size() - 1].flow = (from1 == start ? -capacity1 : 0);
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

	Edge getEdgeOnPosition(size_t u, size_t pos) {
		return edge[u][pos];
	}
};

#endif