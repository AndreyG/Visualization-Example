#pragma once
#include "triangulation_types.h"
#include "algorithms_geom.h"
#include <vector>
#include <map>

using namespace std;

class DCEL {

public:
		
	class Edge {
		friend DCEL;
	private:
		Edge* next_;
		Edge* twin_;
		const PolygonVertex* to_;
	public:
		inline Edge* next() const {
			return this->next_;
		}
		inline Edge* twin() const {
			return this->twin_;
		}
		inline Edge* right_next() const {
			return this->twin()->next();
		}
		inline const PolygonVertex* to() const {
			return this->to_;
		}
		inline const PolygonVertex* from() const {
			return this->twin()->to();
		}
	};

	void add_vertex(const PolygonVertex* v);
	void add_segment(const PolygonVertex* u, const PolygonVertex* v);
	vector<polygon_type> get_all_facets() const;
	~DCEL();

private:
	vector<Edge*> edges;
	// map from vertex to first edge
	map<const PolygonVertex*, Edge*> vertexEdge;
	void insert_new_edge(Edge*);

};
