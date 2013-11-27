#include "dcel.h"
#include "triangulation_types.h"
#include "algorithms_geom.h"
#include <vector>
#include <stdexcept>
#include <math.h>

using namespace std;

double angle2pi(const DCEL::Edge* e) {
	double x = e->to()->point.x - e->from()->point.x;
	double y = e->to()->point.y - e->from()->point.y;

	double piAngle = atan2(y, x);
	if (piAngle < 0) {
		piAngle += M_PI;
	}
	return piAngle;
}

void DCEL::add_vertex(const PolygonVertex* v) {
	if (vertexEdge.find(v) != vertexEdge.end())
		throw logic_error("Vertex is already added.");
	vertexEdge[v] = NULL;
}

void DCEL::add_segment(const PolygonVertex* u, const PolygonVertex* v) {
	Edge* ev = new Edge();
	Edge* eu = new Edge();
	ev->to_ = v;
	eu->to_ = u;
	ev->twin_ = eu;
	eu->twin_ = eu;
	edges.push_back(ev);
	edges.push_back(eu);
	insert_new_edge(ev);
	insert_new_edge(eu);
}

void DCEL::insert_new_edge(Edge* e) {
	const PolygonVertex* from = e->from();
	if (vertexEdge[from] == NULL) {
		vertexEdge[from] = e;
		e->twin()->next_ = e;
		return;
	}
	Edge* firstEdge = vertexEdge[from];
	Edge* curEdge = firstEdge;
	Edge* curBeforeEdge = firstEdge;
	do {
		double aCur = angle2pi(curEdge);
		double aMe = angle2pi(e);
		if (aMe < aCur) {
			break;
		}
		curBeforeEdge = curEdge;
		curEdge = curEdge->right_next();
	} while (curEdge != firstEdge);

	curBeforeEdge->twin()->next_ = e;
	e->twin()->next_ = curEdge;
}

vector<polygon_type> DCEL::get_all_facets() const {
	vector<polygon_type> res;
	return res;
}

DCEL::~DCEL() {
	for (auto ep : edges) {
		delete ep;
	}
}
