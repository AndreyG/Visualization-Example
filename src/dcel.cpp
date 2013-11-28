#include "dcel.h"
#include "triangulation_types.h"
#include "algorithms_geom.h"
#include <vector>
#include <stdexcept>
#include <math.h>

using namespace std;
using namespace geom::algorithms;

double angle2pi(const DCEL::Edge* e) {
	double x = e->to()->point.x - e->from()->point.x;
	double y = e->to()->point.y - e->from()->point.y;
	double piAngle = atan2(y, x);
	if (piAngle < 0) {
		piAngle += 2 * M_PI;
	}
	return piAngle;
}

bool between(const DCEL::Edge* e, const DCEL::Edge* ec) {
	double aa = angle2pi(e);
	double ab = angle2pi(e->right_next());
	double a = angle2pi(ec);
	return ab < a && a < aa;
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
	eu->twin_ = ev;
	edges.push_back(ev);
	edges.push_back(eu);

	cout << "create edge " << ev;
	cout << " (" << ev->from()->point.x << ", " << ev->from()->point.y << ") -> ";
	cout << "(" << ev->to()->point.x << ", " << ev->to()->point.y << ") " << endl;
	cout << "create edge " << eu;
	cout << " (" << eu->from()->point.x << ", " << eu->from()->point.y << ") -> ";
	cout << "(" << eu->to()->point.x << ", " << eu->to()->point.y << ") " << endl;

	insert_new_edge(ev);
	insert_new_edge(eu);
}

void DCEL::insert_new_edge(Edge* edge) {
	const PolygonVertex* from = edge->from();
	if (vertexEdge[from] == NULL) {
		vertexEdge[from] = edge;
		edge->right_next(edge);
		return;
	}
	auto edges = get_all_edges(from);
	double minAngle = 2 * M_PI;
	double maxAngle = 0;
	Edge* minEdge = NULL;
	for (auto e : edges) {
		double myAngle = angle2pi(e);
		if (myAngle < minAngle) {
			minAngle = myAngle;
			minEdge = e;
		}
	}
	maxAngle = angle2pi(minEdge->right_next());

	double eangle = angle2pi(edge);
	Edge* prevEdge = NULL;
	if (eangle > maxAngle || eangle < minAngle) {
		prevEdge = minEdge;
	} else {
		for (auto e : edges) {
			auto angleBefore = angle2pi(e);
			auto angleAfter = angle2pi(e->right_next());
			if (angleBefore > eangle && angleAfter < eangle) {
				prevEdge = e;
				break;
			}
		}
	}

	edge->right_next(prevEdge->right_next());
	prevEdge->right_next(edge);

}

vector<DCEL::Edge*> DCEL::get_all_edges(const PolygonVertex* v) const {
	vector<DCEL::Edge*> res;
	Edge* firstEdge = vertexEdge.find(v)->second;
	if (firstEdge == NULL)
		return res;
	Edge* curEdge = firstEdge;
	while (true) {
		res.push_back(curEdge);
		curEdge = curEdge->right_next();
		if (curEdge == firstEdge)
			break;
	}
	return res;
}

polygon_type DCEL::walk(Edge* e) const {
	polygon_type res;
	while (!e->visited_) {
		e->visited_ = true;
		res.push_back(e->to()->point);
		e = e->next();
	}
	return res;
}

vector<polygon_type> DCEL::get_all_facets(
		const vector<PolygonVertex*>& outerPolygon) const {
	vector<polygon_type> res;

	for (auto e : edges) {
		e->visited_ = false;
	}

	for (auto pv : outerPolygon) {
		auto edges = get_all_edges(pv);
		for (auto e : edges) {
			if (e->visited_)
				continue;
			res.push_back(walk(e));
		}
	}

	return res;
}

DCEL::~DCEL() {
	for (auto ep : edges) {
		delete ep;
	}
}
