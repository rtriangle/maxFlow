#ifndef BFSforAll_h
#define BFSforAll_h
#include <vector>
#include <queue>

using namespace std;

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

#endif