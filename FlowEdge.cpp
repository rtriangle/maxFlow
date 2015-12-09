#ifndef FlowEdge_cpp
#define FlowEdge_cpp

#include "FlowEdge.h"

FlowEdge::FlowEdge(size_t to1, long long capacity1, size_t reversedEdgeNumber1, bool reverse1) :
	to(to1), capacity(capacity1), reversedEdgeNumber(reversedEdgeNumber1), reverse(reverse1) {}

size_t FlowEdge::getTo() {
	return to;
}

size_t FlowEdge::getReversedEdgeNumber() {
	return reversedEdgeNumber;
}

bool FlowEdge::isReversed() {
	return reverse;
}

long long FlowEdge::getCapacity() {
	return capacity;
}

long long FlowEdge::getFLow() {
	return flow;
}

#endif