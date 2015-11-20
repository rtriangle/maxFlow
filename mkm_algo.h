#ifndef mkm_algo_h
#define mkm_algo_h

#include <vector>
#include <queue>
#include <list>
#include "network_mkm.h"

const long long INF = 1e16;

using namespace std;

class MKMAlgo {
private:
	Network NetworkGraph;
	std::vector <bool> deletedVerteces;
	std::vector <long long> inPotential, outPotential, Potential;
	std::vector <size_t> currentEdge, level;

	void updatePotential(size_t v) {
		Potential[v] = std::min(inPotential[v], outPotential[v]);
	}

	void calculateVertexPotential(size_t u) {
		if (u == NetworkGraph.getStart()) {
			inPotential[u] = INF;
		}
		else {
			inPotential[u] = 0;
		}
		if (u == NetworkGraph.getFinish()) {
			outPotential[u] = INF;
		}
		else {
			outPotential[u] = 0;
		}
		for (size_t itEdge = 0; itEdge < NetworkGraph.getVertexDegree(u); ++itEdge) {
			size_t v = NetworkGraph.getEdgeToOnPosition(u, itEdge);
			if (level[u] == level[v] + 1) {
				inPotential[u] += NetworkGraph.getCapReversedEdge(u, itEdge) - NetworkGraph.getFlowReversedEdge(u, itEdge);
			}
			if (level[u] + 1 == level[v]) {
				outPotential[u] += NetworkGraph.getCap(u, itEdge) - NetworkGraph.getFlow(u, itEdge);
			}
		}
		updatePotential(u);
	}

	void BFS(size_t u){
		int numbV = NetworkGraph.getNumberOfVertices();
		level = std::vector <size_t>(numbV + 1, numbV + 1);
		std::queue <size_t> bfsQ;
		bfsQ.push(u);
		level[u] = 0;
		while (!bfsQ.empty()) {
			size_t curVert = bfsQ.front();
			bfsQ.pop();
			size_t curDeg = NetworkGraph.getVertexDegree(curVert);
			for (size_t itEdge = 0; itEdge < curDeg; ++itEdge) {
				size_t v = NetworkGraph.getEdgeToOnPosition(curVert, itEdge);
				//size_t capacity = NetworkGraph.getCap(curVert, itEdge);
				//size_t flow = NetworkGraph.getFlow(curVert, itEdge);
				if (level[v] == (numbV + 1) && (NetworkGraph.getCap(curVert, itEdge) - NetworkGraph.getFlow(curVert, itEdge)) > 0) {
					level[v] = level[curVert] + 1;
					bfsQ.push(v);
				}
			}
		}
	}

	long long pushByEdge(size_t u, size_t number, long long flowToPush) {
		//long long cap = NetworkGraph.getCap(u, number);
		//long long flow = NetworkGraph.getFlow(u, number);
		long long realPushedFlow = std::min(flowToPush, NetworkGraph.getCap(u, number) - NetworkGraph.getFlow(u, number));
		size_t revNumber = NetworkGraph.getReversedEdgeNumber(u, number);
		size_t v = NetworkGraph.getEdgeToOnPosition(u, number);
		//long long curFlow = NetworkGraph.getFlow(u, number);
		//long long curFlowRev = NetworkGraph.getFlow(v, revNumb);
		if (!NetworkGraph.getEdgeReversed(u, number)) {

			NetworkGraph.setFlow(u, number, NetworkGraph.getFlow(u, number) + realPushedFlow);
			NetworkGraph.setFlow(v, revNumber, NetworkGraph.getFlow(v, revNumber) - realPushedFlow);
		}
		else {
			NetworkGraph.setFlow(u, number, NetworkGraph.getFlow(u, number) + realPushedFlow);
			NetworkGraph.setFlow(v, revNumber, NetworkGraph.getFlow(v, revNumber) - realPushedFlow);
		}
		return realPushedFlow;
	}

	void sendFlow(size_t v, long long sizeFlow, bool toStart) {
		std::queue <size_t> queueFlow;
		std::vector <long long> extra(NetworkGraph.getNumberOfVertices() + 1, 0);
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

			if (toStart && curVert == NetworkGraph.getStart() ||
				!toStart && curVert == NetworkGraph.getFinish()) {
				return;
			}

			while (extra[curVert] > 0) {
				size_t nextVert = NetworkGraph.getEdgeToOnPosition(curVert, currentEdge[curVert]);
				bool edgeIsOk = false;
				size_t from, numberOfEdge;
				if (toStart && !deletedVerteces[nextVert] && level[nextVert] + 1 == level[curVert]) {
					edgeIsOk = true;
					from = nextVert;
					numberOfEdge = NetworkGraph.getReversedEdgeNumber(curVert, currentEdge[curVert]);
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

				currentEdge[curVert] = (currentEdge[curVert] + 1) % NetworkGraph.getVertexDegree(curVert);
			}
		}
	}

	void makeFlowThrough(size_t v) {
		long long sizeFlow = Potential[v];
		sendFlow(v, sizeFlow, true);
		sendFlow(v, sizeFlow, false);
	}

	void deleteVertex(size_t u) {
		deletedVerteces[u] = true;
		for (size_t itEdge = 0; itEdge < NetworkGraph.getVertexDegree(u); ++itEdge) {
			size_t v = NetworkGraph.getEdgeToOnPosition(u, itEdge);
			//long long capacity = NetworkGraph.getCap(u, itEdge);
			//long long flow1 = NetworkGraph.getFlow(u, itEdge);
			//long long curFlowRev = NetworkGraph.getFlow(u, itEdge);
			//long long revCap = NetworkGraph.getCapReversedEdge(u, itEdge);
			if (level[v] == level[u] + 1) {
				inPotential[v] -= (NetworkGraph.getCap(u, itEdge) - NetworkGraph.getFlow(u, itEdge));
			}
			if (level[v] + 1 == level[u]) {
				outPotential[v] -= (NetworkGraph.getCapReversedEdge(u, itEdge) + NetworkGraph.getFlow(u, itEdge));
			}
			updatePotential(v);
		}
	}

	void resizePotentials() {
		Potential.resize(NetworkGraph.getNumberOfVertices() + 1);
		inPotential.resize(NetworkGraph.getNumberOfVertices() + 1);
		outPotential.resize(NetworkGraph.getNumberOfVertices() + 1);
	}

public:

	void findMaxFlow(Network networkGraph) {
		NetworkGraph = networkGraph;
		bool doIt = true;
		resizePotentials();
		while (doIt) {
			doIt = false;
			BFS(NetworkGraph.getStart());
			if (level[NetworkGraph.getFinish()] != NetworkGraph.getNumberOfVertices() + 1) {
				doIt = true;
			}
			else {
				break;
			}
			currentEdge = std::vector <size_t>(NetworkGraph.getNumberOfVertices(), 0);
			deletedVerteces = std::vector < bool >(NetworkGraph.getNumberOfVertices() + 1, false);
			for (size_t vertex = 1; vertex <= NetworkGraph.getNumberOfVertices(); vertex++) {
				calculateVertexPotential(vertex);
			}
			for (size_t i = 1; i <= NetworkGraph.getNumberOfVertices(); ++i) {
				size_t keyVertex = NetworkGraph.getNumberOfVertices() + 1;
				for (size_t vertex = 1; vertex <= NetworkGraph.getNumberOfVertices(); ++vertex)
					if (!deletedVerteces[vertex] && (keyVertex == NetworkGraph.getNumberOfVertices() + 1 || Potential[vertex] < Potential[keyVertex])) {
						keyVertex = vertex;
					}
				if (Potential[keyVertex] != 0) {
					makeFlowThrough(keyVertex);
				}
				deleteVertex(keyVertex);
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