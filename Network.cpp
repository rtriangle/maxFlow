#ifndef Network_mkm_cpp
#define Network_mkm_cpp
#include "Network.h"
#include <vector>

Network::Network(size_t n, size_t start1, size_t finish1) {
		edge.resize(n + 1);
		numberOfVertices = n;
		numberOfEdges = 0;
		start = start1;
		finish = finish1;
	}

void Network::addEdge(size_t from, size_t to, long long capacity) {
	edge[from].push_back(FlowEdge(to, capacity, edge[to].size(), false));
	edge[to].push_back(FlowEdge(from, 0, edge[from].size() - 1, true));
	edge[from][edge[from].size() - 1].flow = 0;
	edge[to][edge[to].size() - 1].flow = 0;
	++numberOfEdges;
}

size_t Network::getNumberOfVertices() {
	return numberOfVertices;
}

size_t Network::getNumberOfEdges() {
	return numberOfEdges;
}

size_t Network::getStart() {
	return start;
}

size_t Network::getFinish() {
	return finish;
}

size_t Network::getEdgeToOnPosition(size_t v, size_t pos) {
	if (pos < edge[v].size()) {
		return edge[v][pos].to;
	}
	else
		return 0;
}

size_t Network::getVertexDegree(size_t v) {
	return edge[v].size();
}

size_t Network::getReversedEdgeNumber(size_t u, size_t number) {
	return edge[u][number].reversedEdgeNumber;
}

long long Network::getCap(size_t v, size_t number) {
	return edge[v][number].capacity;
}

void Network::setCap(size_t v, size_t number, long long _capacity) {
	edge[v][number].capacity = _capacity;
}

	//v->v[number] returns cap(v[number] -> v)
long long Network::getCapReversedEdge(size_t v, size_t number) {
	size_t v1 = edge[v][number].to;
	size_t rNumber = edge[v][number].reversedEdgeNumber;
	return edge[v1][rNumber].capacity;
}

long long Network::getFlowReversedEdge(size_t v, size_t number) {
	size_t v1 = edge[v][number].to;
	size_t rNumber = edge[v][number].reversedEdgeNumber;
	return edge[v1][rNumber].flow;
}

long long Network::getFlow(size_t v, size_t number) {
	return edge[v][number].flow;
}

void Network::setFlow(size_t v, size_t number, long long _flow) {
	edge[v][number].flow = _flow;
}

bool Network::getEdgeReversed(size_t v, size_t number) {
	return edge[v][number].reverse;
}

FlowEdge Network::getEdgeOnPosition(size_t u, size_t pos) {
	return edge[u][pos];
}

#endif