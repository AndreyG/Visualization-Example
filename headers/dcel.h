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
		bool visited_;
		Edge(){
			next_ = NULL;
			twin_ = NULL;
			to_ = NULL;
			visited_ = false;
		}
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
		inline Edge* right_next(Edge* e) const {
			return this->twin()->next_ = e;
		}
		inline const PolygonVertex* to() const {
			return this->to_;
		}
		inline const PolygonVertex* from() const {
			return this->twin()->to();
		}
		inline segment_type get_segment() const {
			return segment_type(from()->point, to()->point);
		}
	};

	void add_vertex(const PolygonVertex* v);
	Edge* add_segment(const PolygonVertex* u, const PolygonVertex* v);
	vector<DCEL::Edge*> get_all_edges (const PolygonVertex* v) const;
	vector<polygon_type> get_all_facets(const vector<DCEL::Edge*>&) const;
	~DCEL();

private:
	vector<Edge*> edges;
	// map from vertex to first edge
	map<const PolygonVertex*, Edge*> vertexEdge;
	void insert_new_edge(Edge*);
	polygon_type walk(Edge*) const;

};
