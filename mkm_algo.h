#ifndef mkm_algo_h
#define mkm_algo_h

#include <vector>
#include <queue>
#include <list>
#include "network_mkm.h"

using namespace std;

bool EdgeChecker(long long capacity, long long flow) {
	return capacity - flow > 0;
}

template <typename EdgeType, typename Graph, typename Function>
vector <size_t> BFSforAll(size_t start, Graph graph, Function functionEdge) {
	int numbV = graph.getNumberOfVertices();
	vector <size_t> level(numbV + 1, numbV + 1);
	std::queue <size_t> bfsQ;
	bfsQ.push(start);
	level[start] = 0;
	while (!bfsQ.empty()) {
		size_t curVert = bfsQ.front();
		bfsQ.pop();
		size_t curDeg = graph.getVertexDegree(curVert);
		for (size_t itEdge = 0; itEdge < curDeg; ++itEdge) {
			EdgeType e = graph.getEdgeOnPosition(curVert, itEdge);
			if (level[e.to] == (numbV + 1) && functionEdge(e.capacity, e.flow)) {
				level[e.to] = level[curVert] + 1;
				bfsQ.push(e.to);
			}
		}
	}
	return level;
}

class MKMAlgo {
private:
	std::vector <bool> deletedVerteces;
	std::vector <long long> inPotential, outPotential, potential;
	std::vector <size_t> currentEdge, level;
	static const long long INF = 1e16;

	void updatePotential(size_t v) {
		potential[v] = std::min(inPotential[v], outPotential[v]);
	}

	void calculateVertexPotential(size_t u) {
		if (u == networkGraph.getStart()) {
			inPotential[u] = INF;
		}
		else {
			inPotential[u] = 0;
		}
		if (u == networkGraph.getFinish()) {
			outPotential[u] = INF;
		}
		else {
			outPotential[u] = 0;
		}
		for (size_t itEdge = 0; itEdge < networkGraph.getVertexDegree(u); ++itEdge) {
			size_t v = networkGraph.getEdgeToOnPosition(u, itEdge);
			if (level[u] == level[v] + 1) {
				inPotential[u] += networkGraph.getCapReversedEdge(u, itEdge) - networkGraph.getFlowReversedEdge(u, itEdge);
			}
			if (level[u] + 1 == level[v]) {
				outPotential[u] += networkGraph.getCap(u, itEdge) - networkGraph.getFlow(u, itEdge);
			}
		}
		updatePotential(u);
	}

	void BFS(size_t u) {
		/*
		int numbV = networkGraph.getNumberOfVertices();
		level = std::vector <size_t>(numbV + 1, numbV + 1);
		std::queue <size_t> bfsQ;
		bfsQ.push(u);
		level[u] = 0;
		while (!bfsQ.empty()) {
			size_t curVert = bfsQ.front();
			bfsQ.pop();
			size_t curDeg = networkGraph.getVertexDegree(curVert);
			for (size_t itEdge = 0; itEdge < curDeg; ++itEdge) {
				size_t v = networkGraph.getEdgeToOnPosition(curVert, itEdge);
				//size_t capacity = networkGraph.getCap(curVert, itEdge);
				//size_t flow = networkGraph.getFlow(curVert, itEdge);
				if (level[v] == (numbV + 1) && (networkGraph.getCap(curVert, itEdge) - networkGraph.getFlow(curVert, itEdge)) > 0) {
					level[v] = level[curVert] + 1;
					bfsQ.push(v);
				}
			}
		}
		*/
		level = BFSforAll<Edge>(1, networkGraph, EdgeChecker);
	}

	long long pushByEdge(size_t u, size_t number, long long flowToPush) {
		//long long cap = networkGraph.getCap(u, number);
		//long long flow = networkGraph.getFlow(u, number);
		long long realPushedFlow = std::min(flowToPush, networkGraph.getCap(u, number) - networkGraph.getFlow(u, number));
		size_t revNumber = networkGraph.getReversedEdgeNumber(u, number);
		size_t v = networkGraph.getEdgeToOnPosition(u, number);
		//long long curFlow = networkGraph.getFlow(u, number);
		//long long curFlowRev = networkGraph.getFlow(v, revNumb);
		if (!networkGraph.getEdgeReversed(u, number)) {

			networkGraph.setFlow(u, number, networkGraph.getFlow(u, number) + realPushedFlow);
			networkGraph.setFlow(v, revNumber, networkGraph.getFlow(v, revNumber) - realPushedFlow);
		}
		else {
			networkGraph.setFlow(u, number, networkGraph.getFlow(u, number) + realPushedFlow);
			networkGraph.setFlow(v, revNumber, networkGraph.getFlow(v, revNumber) - realPushedFlow);
		}
		return realPushedFlow;
	}

	void sendFlow(size_t v, long long sizeFlow, bool toStart) {
		std::queue <size_t> queueFlow;
		std::vector <long long> extra(networkGraph.getNumberOfVertices() + 1, 0);
		queueFlow.push(v);
		extra[v] = sizeFlow;

		while (!queueFlow.empty()) {
			size_t curVert = queueFlow.front();
			queueFlow.pop();

			if (toStart) {
				inPotential[curVert] -= extra[curVert];
			}
			else {
				outPotential[curVert] -= extra[curVert];
			}
			updatePotential(curVert);

			if (toStart && curVert == networkGraph.getStart() ||
				!toStart && curVert == networkGraph.getFinish()) {
				return;
			}

			while (extra[curVert] > 0) {
				size_t nextVert = networkGraph.getEdgeToOnPosition(curVert, currentEdge[curVert]);
				bool edgeIsOk = false;
				size_t from, numberOfEdge;
				if (toStart && !deletedVerteces[nextVert] && level[nextVert] + 1 == level[curVert]) {
					edgeIsOk = true;
					from = nextVert;
					numberOfEdge = networkGraph.getReversedEdgeNumber(curVert, currentEdge[curVert]);
				}
				else {
					if (!toStart && !deletedVerteces[nextVert] && level[nextVert] == level[curVert] + 1) {
						from = curVert;
						numberOfEdge = currentEdge[curVert];
						edgeIsOk = true;
					}
				}
				if (edgeIsOk) {
					size_t curSizePush = pushByEdge(from, numberOfEdge, extra[curVert]);
					if (extra[nextVert] == 0)
						queueFlow.push(nextVert);
					extra[nextVert] += curSizePush;
					extra[curVert] -= curSizePush;
					if (toStart)
						outPotential[nextVert] -= curSizePush;
					else
						inPotential[nextVert] -= curSizePush;
					updatePotential(nextVert);
				}

				currentEdge[curVert] = (currentEdge[curVert] + 1) % networkGraph.getVertexDegree(curVert);
			}
		}
	}

	void makeFlowThrough(size_t v) {
		long long sizeFlow = potential[v];
		sendFlow(v, sizeFlow, true);
		sendFlow(v, sizeFlow, false);
	}

	void deleteVertex(size_t u) {
		deletedVerteces[u] = true;
		for (size_t itEdge = 0; itEdge < networkGraph.getVertexDegree(u); ++itEdge) {
			size_t v = networkGraph.getEdgeToOnPosition(u, itEdge);
			//long long capacity = networkGraph.getCap(u, itEdge);
			//long long flow1 = networkGraph.getFlow(u, itEdge);
			//long long curFlowRev = networkGraph.getFlow(u, itEdge);
			//long long revCap = networkGraph.getCapReversedEdge(u, itEdge);
			if (level[v] == level[u] + 1) {
				inPotential[v] -= (networkGraph.getCap(u, itEdge) - networkGraph.getFlow(u, itEdge));
			}
			if (level[v] + 1 == level[u]) {
				outPotential[v] -= (networkGraph.getCapReversedEdge(u, itEdge) + networkGraph.getFlow(u, itEdge));
			}
			updatePotential(v);
		}
	}

	void resizePotentials() {
		potential.resize(networkGraph.getNumberOfVertices() + 1);
		inPotential.resize(networkGraph.getNumberOfVertices() + 1);
		outPotential.resize(networkGraph.getNumberOfVertices() + 1);
	}

public:
	Network networkGraph;
	vector < pair <size_t, size_t> > inputEdgesOrder;

	MKMAlgo(vector < pair <size_t, size_t> > &inputEdgesOrderIN) {
		inputEdgesOrder = inputEdgesOrderIN;
	}

	void findMaxFlow(Network networkGraphInput) {
		networkGraph = networkGraphInput;
		bool doIt = true;
		resizePotentials();
		while (doIt) {
			doIt = false;
			BFS(networkGraph.getStart());
			if (level[networkGraph.getFinish()] != networkGraph.getNumberOfVertices() + 1) {
				doIt = true;
			}
			else {
				break;
			}
			currentEdge = std::vector <size_t>(networkGraph.getNumberOfVertices(), 0);
			deletedVerteces = std::vector < bool >(networkGraph.getNumberOfVertices() + 1, false);
			for (size_t vertex = 1; vertex <= networkGraph.getNumberOfVertices(); vertex++) {
				calculateVertexPotential(vertex);
			}
			for (size_t i = 1; i <= networkGraph.getNumberOfVertices(); ++i) {
				size_t keyVertex = networkGraph.getNumberOfVertices() + 1;
				for (size_t vertex = 1; vertex <= networkGraph.getNumberOfVertices(); ++vertex)
					if (!deletedVerteces[vertex] && (keyVertex == networkGraph.getNumberOfVertices() + 1 || potential[vertex] < potential[keyVertex])) {
						keyVertex = vertex;
					}
				if (potential[keyVertex] != 0) {
					makeFlowThrough(keyVertex);
				}
				deleteVertex(keyVertex);
			}
		}
	}
	void output(std::vector < std::pair <size_t, size_t> > inputEdgesOrder) {
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

};

ostream& operator << (ostream& stream, MKMAlgo algo) {
	long long resFlow = 0;
	for (int i = 0; i < algo.networkGraph.getVertexDegree(1); ++i)
		if (algo.networkGraph.getEdgeToOnPosition(1, i) != 1) {
			resFlow += algo.networkGraph.getFlow(1, i);
		}
	cout << resFlow << '\n';
	for (int i = 0; i < algo.inputEdgesOrder.size(); ++i) {
		if (algo.inputEdgesOrder[i].first != -1)
			cout << algo.networkGraph.getFlow(algo.inputEdgesOrder[i].first, algo.inputEdgesOrder[i].second) << "\n";
		else
			cout << "0\n";
	}
	return stream;
}

#endif