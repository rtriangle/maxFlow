#ifndef Network_push_relable_cpp
#define Network_push_relable_algo_cpp

#include "Network_push_ralable.h"
#include <vector>

using namespace std;

Network::Network(size_t n, size_t start1, size_t finish1) {
	edge.resize(n + 1);
	numberOfVertices = n;
	numberOfEdges = 0;
	start = start1;
	finish = finish1;
}

Network::Network(){}

void Network::addEdge(size_t from1, size_t to1, long long capacity1) {
	edge[from1].push_back(Edge(to1, capacity1, edge[to1].size()));
	edge[to1].push_back(Edge(from1, 0, edge[from1].size() - 1));
	if (from1 == start) {
		edge[from1][edge[from1].size() - 1].flow = (from1 == start ? capacity1 : 0);
		edge[to1][edge[to1].size() - 1].flow = (from1 == start ? -capacity1 : 0);
	}
	++numberOfEdges;
}

long long Network::getEdgeFlow(size_t from, size_t to) {
	return edge[from][to].flow;
}

size_t Network::getStart() {
	return start;
}

size_t Network::getFinish() {
	return finish;
}

size_t Network::getEdgeToOnPosition(size_t v, size_t pos) {
	if (pos < edge[v].size())
		return edge[v][pos].to;
	else
		return 0;
}

size_t Network::getVertexDegree(size_t v) {
	return edge[v].size();
}

long long Network::getCap(size_t v, size_t number) {
	return edge[v][number].capacity;
}

long long Network::getFlow(size_t v, size_t number) {
	return edge[v][number].flow;
}

size_t Network::getNumberOfVertices() {
	return numberOfVertices;
}

size_t Network::getNumberOfEdges() {
	return numberOfEdges;
}

void Network::changeFlow(size_t u, size_t pos, long long addflow) {
	edge[u][pos].flow += addflow;
	size_t tmp = edge[u][pos].reversedEdge;
	edge[edge[u][pos].to][tmp].flow -= addflow;
}

Edge Network::getEdgeOnPosition(size_t u, size_t pos) {
	return edge[u][pos];
}

#endif