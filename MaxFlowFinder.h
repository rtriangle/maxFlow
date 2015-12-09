#ifndef MaxFlowFinder_h
#define MaxFlowFinder_h
#include "Network.h"

using namespace std;

class MaxFlowFinder {
public:
	virtual void findMaxFlow() = 0;
	Network network;
	MaxFlowFinder(Network &network);
};

#endif