#ifndef Graph_h
#define Graph_h

using namespace std;

class Graph {
public:
	virtual size_t getNumberOfVertices() = 0;
	virtual size_t getVertexDegree(size_t vertex) = 0;
};

#endif