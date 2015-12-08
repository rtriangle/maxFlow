#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <cstdlib>
#include <algorithm>
#include "mkm_algo.h"
//#include "push_relable_algo.h"
#pragma warning(disable:4996)

using namespace std;

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int v, e;
	std::cin >> v >> e;
	Network networkGraph(v + 1, 1, v);
	vector < pair <size_t, size_t> > inputEdgesOrder;
	size_t from, to;
	long long capacity;
	for (size_t i = 0; i < e; ++i) {
		std::cin >> from >> to >> capacity;
		if (from == to)
			inputEdgesOrder.push_back(std::make_pair(-1, -1));
		else
			inputEdgesOrder.push_back(std::make_pair(from, networkGraph.getVertexDegree(from)));
		if (from != to) {
			networkGraph.addEdge(from, to, capacity);
		}
	}
	MKMAlgo mkm(inputEdgesOrder);
	mkm.findMaxFlow(networkGraph);
	mkm.output(inputEdgesOrder);
//	cout << dischargeAlgo;
	fclose(stdin);
	fclose(stdout);
	return 0;
}