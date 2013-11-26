#pragma once
#include "triangulation_types.h"
#include "algorithms_geom.h"
#include <vector>
#include <map>

using namespace std;

class DCEL {

public:
		
	struct Edge {
		Edge* next;
		Edge* twin;
		const PolygonVertex* to;
	};

	void add_segment(const PolygonVertex* u, const PolygonVertex* v);
	vector<polygon_type> get_all_facets();

private:
	vector<Edge> edges;
	// map from vertex to first edge
	map<const PolygonVertex*, Edge*> vertexEdge;
	void insert_new_edge(Edge*);

};
