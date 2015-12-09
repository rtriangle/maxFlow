#ifndef MKMAlgo_cpp
#define MKMAlgo_cpp

#include <vector>
#include <queue>
#include <list>
#include <algorithm>
#include "MKMAlgo.h"
#include "ProjectAdditionalFunctions.h"

using namespace std;

void MKMAlgo::updatePotential(size_t v) {
	potential[v] = std::min(inPotential[v], outPotential[v]);
}

void MKMAlgo::calculateVertexPotential(size_t u) {
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

long long MKMAlgo::pushByEdge(size_t u, size_t number, long long flowToPush) {
	long long realPushedFlow = std::min(flowToPush, networkGraph.getCap(u, number) - networkGraph.getFlow(u, number));
	size_t revNumber = networkGraph.getReversedEdgeNumber(u, number);
	size_t v = networkGraph.getEdgeToOnPosition(u, number);
	if (!networkGraph.getEdgeReversed(u, number)) {
		networkGraph.setFlow(u, number, networkGraph.getFlow(u, number) + realPushedFlow);
		networkGraph.setFlow(v, revNumber, networkGraph.getFlow(v, revNumber) - realPushedFlow);
	} else {
		networkGraph.setFlow(u, number, networkGraph.getFlow(u, number) + realPushedFlow);
		networkGraph.setFlow(v, revNumber, networkGraph.getFlow(v, revNumber) - realPushedFlow);
	}
	return realPushedFlow;
}

void MKMAlgo::sendFlow(size_t v, long long sizeFlow, bool toStart) {
	std::queue <size_t> queueFlow;
	std::vector <long long> extra(networkGraph.getNumberOfVertices() + 1, 0);
	queueFlow.push(v);
	extra[v] = sizeFlow;
	while (!queueFlow.empty()) {
		size_t curVert = queueFlow.front();
		queueFlow.pop();
		if (toStart) {
			inPotential[curVert] -= extra[curVert];
		} else {
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
			} else {
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

void MKMAlgo::makeFlowThrough(size_t v) {
	long long sizeFlow = potential[v];
	sendFlow(v, sizeFlow, true);
	sendFlow(v, sizeFlow, false);
}

void MKMAlgo::deleteVertex(size_t u) {
	deletedVerteces[u] = true;
	for (size_t itEdge = 0; itEdge < networkGraph.getVertexDegree(u); ++itEdge) {
		size_t v = networkGraph.getEdgeToOnPosition(u, itEdge);
		if (level[v] == level[u] + 1) {
			inPotential[v] -= (networkGraph.getCap(u, itEdge) - networkGraph.getFlow(u, itEdge));
		}
		if (level[v] + 1 == level[u]) {
			outPotential[v] -= (networkGraph.getCapReversedEdge(u, itEdge) + networkGraph.getFlow(u, itEdge));
		}
		updatePotential(v);
	}
}

void MKMAlgo::resizePotentials() {
	potential.resize(networkGraph.getNumberOfVertices() + 1);
	inPotential.resize(networkGraph.getNumberOfVertices() + 1);
	outPotential.resize(networkGraph.getNumberOfVertices() + 1);
}

MKMAlgo::MKMAlgo(vector < pair <size_t, size_t> > &inputEdgesOrderIN) {
	inputEdgesOrder = inputEdgesOrderIN;
}

void MKMAlgo::findMaxFlow(Network networkGraphInput) {
	networkGraph = networkGraphInput;
	bool doIt = true;
	resizePotentials();
	while (doIt) {
		doIt = false;
		maxFlowProject::graphMethods::BFSforAll<FlowEdge>(size_t(0), networkGraph, maxFlowProject::edgeFunctions::EdgeChecker);
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
	
void MKMAlgo::output(std::vector < std::pair <size_t, size_t> > inputEdgesOrder) {
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