#ifndef Push_relable_algo_cpp
#define Push_relable_algo_cpp

#include <vector>
#include <queue>
#include <list>
#include "PushRelableAlgo.h"

using namespace std;

void PushRelabelAlgo::initialize() {
	size_t numVert = networkGraph.getNumberOfVertices() - 1;
	excess.resize(numVert + 1, 0);
	height.assign(numVert + 1, 0);
	currentEdge.assign(numVert + 1, 0);
	height[networkGraph.getStart()] = numVert;
	for (int i = 2; i < numVert; ++i) {
		verteces.push_back(i);
	}
	for (int i = 0; i < networkGraph.getVertexDegree(1); ++i) {
		excess[networkGraph.getEdgeToOnPosition(1, i)] += networkGraph.getCap(1, i);
	}
}

void PushRelabelAlgo::discharge(size_t v, Network& networkGraph) {
	while (excess[v] > 0) {
		if (currentEdge[v] == networkGraph.getVertexDegree(v)) {
			relabel(v, networkGraph.getVertexDegree(v), networkGraph);
		}
		else
			if (push(v, networkGraph)) {
				++currentEdge[v];
			}
			else {
				++currentEdge[v];
			}
	}
}

bool PushRelabelAlgo::push(size_t _u, Network& networkGraph) {
	int a = height[networkGraph.getEdgeToOnPosition(_u, currentEdge[_u])];
	int b = height[_u];
	//a <= b -> false
	if ((networkGraph.getCap(_u, currentEdge[_u]) - networkGraph.getFlow(_u, currentEdge[_u])) <= 0 ||
		height[_u] <= height[networkGraph.getEdgeToOnPosition(_u, currentEdge[_u])]) {
		return false;
	}
	long long delta = std::min(excess[_u], networkGraph.getCap(_u, currentEdge[_u]) - networkGraph.getFlow(_u, currentEdge[_u]));
	size_t _v = networkGraph.getEdgeToOnPosition(_u, currentEdge[_u]);
	excess[_u] -= delta;
	excess[_v] += delta;
	networkGraph.changeFlow(_u, currentEdge[_u], delta);
	return true;
}

void PushRelabelAlgo::relabel(size_t _u, size_t degU, Network& networkGraph) {
	currentEdge[_u] = 0;
	size_t minHeight = networkGraph.getNumberOfVertices() * 2;
	std::vector< size_t >::iterator edgeNumberIterator;
	for (int i = 0; i < degU; ++i) {
		int a = networkGraph.getCap(_u, i);
		int b = networkGraph.getFlow(_u, i);
		if (networkGraph.getCap(_u, i) - networkGraph.getFlow(_u, i)) {
			minHeight = std::min(minHeight, height[networkGraph.getEdgeToOnPosition(_u, i)]);
		}
	}
	height[_u] = minHeight + 1;
}

void PushRelabelAlgo::findMaxFlow(Network& networkGraph) {
	networkGraph = networkGraph;
	initialize();
	for (std::list<size_t>::iterator it = verteces.begin(); it != verteces.end(); ++it) {
		size_t oldHeight, vertexNum;
		oldHeight = height[*it];
		discharge(*it, networkGraph);
		if (oldHeight != height[*it]) {
			vertexNum = *it;
			verteces.erase(it);
			verteces.push_front(vertexNum);
			it = verteces.begin();
		}
	}
}

void PushRelabelAlgo::output(std::vector < std::pair <size_t, size_t> > inputEdgesOrder) {
	long long resFlow = 0;
	for (int i = 0; i < networkGraph.getVertexDegree(1); ++i)
		if (networkGraph.getEdgeToOnPosition(1, i) != 1) {
			resFlow += networkGraph.getFlow(1, i);
		}
	cout << resFlow << '\n';
	for (int i = 0; i < inputEdgesOrder.size(); ++i) {
		if (inputEdgesOrder[i].first != -1)
			cout << networkGraph.getFlow(inputEdgesOrder[i].first, inputEdgesOrder[i].second) << "\n";
		else
			cout << "0\n";
	}
}

#endif