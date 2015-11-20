#ifndef push_relable_algo_h
#define push_relable_algo_h

#include <vector>
#include <queue>
#include "network_push_relable.h"
#include <list>
using namespace std;

class PushRelabelAlgo {
private:
	Network NetworkGraph;
	std::vector <size_t> height, currentEdge;
	std::vector <long long> excess;
	size_t doIt;
	std::queue <size_t> q;
	std::list< size_t > verteces;

	void initialize() {
		size_t numVert = NetworkGraph.getNumberOfVertices() - 1;
		excess.resize(numVert + 1, 0);
		height.assign(numVert + 1, 0);
		currentEdge.assign(numVert + 1, 0);
		height[NetworkGraph.getStart()] = numVert;
		for (int i = 2; i < numVert; ++i) {
			verteces.push_back(i);
		}
		for (int i = 0; i < NetworkGraph.getVertexDegree(1); ++i) {
			excess[NetworkGraph.getEdgeToOnPosition(1, i)] += NetworkGraph.getCap(1, i);
		}
	}

	void discharge(size_t v, Network& NetworkGraph) {
		while (excess[v] > 0) {
			if (currentEdge[v] == NetworkGraph.getVertexDegree(v)) {
				relabel(v, NetworkGraph.getVertexDegree(v), NetworkGraph);
			}
			else
				if (push(v, NetworkGraph)) {
					++currentEdge[v];
				}
				else {
					++currentEdge[v];
				}
		}
	}

	bool push(size_t _u, Network& NetworkGraph) {
		int a = height[NetworkGraph.getEdgeToOnPosition(_u, currentEdge[_u])];
		int b = height[_u];
		//a <= b -> false
		if ((NetworkGraph.getCap(_u, currentEdge[_u]) - NetworkGraph.getFlow(_u, currentEdge[_u])) <= 0 ||
			height[_u] <= height[NetworkGraph.getEdgeToOnPosition(_u, currentEdge[_u])]) {
			return false;
		}
		long long delta = std::min(excess[_u], NetworkGraph.getCap(_u, currentEdge[_u]) - NetworkGraph.getFlow(_u, currentEdge[_u]));
		size_t _v = NetworkGraph.getEdgeToOnPosition(_u, currentEdge[_u]);
		excess[_u] -= delta;
		excess[_v] += delta;
		NetworkGraph.changeFlow(_u, currentEdge[_u], delta);
		return true;
	}

	void relabel(size_t _u, size_t degU, Network& NetworkGraph) {
		currentEdge[_u] = 0;
		size_t minHeight = NetworkGraph.getNumberOfVertices() * 2;
		std::vector< size_t >::iterator edgeNumberIterator;
		for (int i = 0; i < degU; ++i) {
			int a = NetworkGraph.getCap(_u, i);
			int b = NetworkGraph.getFlow(_u, i);
			if (NetworkGraph.getCap(_u, i) - NetworkGraph.getFlow(_u, i)) {
				minHeight = std::min(minHeight, height[NetworkGraph.getEdgeToOnPosition(_u, i)]);
			}
		}
		height[_u] = minHeight + 1;
	}

public:
	void findMaxFlow(Network& networkGraph) {
		NetworkGraph = networkGraph;
		initialize();
		for (std::list<size_t>::iterator it = verteces.begin(); it != verteces.end(); ++it) {
			size_t oldHeight, vertexNum;
			oldHeight = height[*it];
			discharge(*it, NetworkGraph);
			if (oldHeight != height[*it]) {
				vertexNum = *it;
				verteces.erase(it);
				verteces.push_front(vertexNum);
				it = verteces.begin();
			}
		}
	}

	void output(std::vector < std::pair <size_t, size_t> > inputEdgesOrder) {
		long long resFlow = 0;
		for (int i = 0; i < NetworkGraph.getVertexDegree(1); ++i)
			if (NetworkGraph.getEdgeToOnPosition(1, i) != 1) {
				resFlow += NetworkGraph.getFlow(1, i);
			}
		cout << resFlow << '\n';
		for (int i = 0; i < inputEdgesOrder.size(); ++i) {
			if (inputEdgesOrder[i].first != -1)
				cout << NetworkGraph.getFlow(inputEdgesOrder[i].first, inputEdgesOrder[i].second) << "\n";
			else
				cout << "0\n";
		}
	}
};

#endif