#ifndef FlowEdge_h
#define FlowEdge_h

#include "Edge.h"

class FlowEdge : public Edge {
public:
	long long capacity, flow;
	size_t to, reversedEdgeNumber;
	bool reverse;
	FlowEdge(size_t to1, long long capacity1, size_t reversedEdgeNumber1, bool reverse1);

	size_t getTo();

	size_t getReversedEdgeNumber();

	bool isReversed();

	long long getCapacity();

	long long getFLow();

};

#endif