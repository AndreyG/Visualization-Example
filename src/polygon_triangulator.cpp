#include "polygon_triangulator.h"
#include "triangulation_status.h"
#include "dcel.h"
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

vector<pair<point_type, TRIP_TYPE> > PolygonTriangulator::get_points_types() {
	vector<pair<point_type, TRIP_TYPE> > res;
	for (auto v : all_vertexes) {
		res.push_back(make_pair(v->point, v->type));
	}
	return res;
}

vector<segment_type> PolygonTriangulator::get_split_segments() {
	vector<segment_type> res;
	for (auto s : splits) {
		res.push_back(s.get_segment());
	}
	return res;
}

vector<segment_type> PolygonTriangulator::get_triangulation_segments() {
	vector<segment_type> res;
	for (auto t : triangulation_segments) {
		res.push_back(t.get_segment());
	}
	return res;
}

void PolygonTriangulator::fill_points_types() {
	for (auto v : all_vertexes) {
		set_trip_type(*v);
	}
}

void PolygonTriangulator::collect_all_vertexes() {
	for (size_t i = 0; i < this->polygon.size(); i++) {
		all_vertexes.push_back(&this->polygon[i]);
	}
	for (size_t k = 0; k < this->holes.size(); k++) {
		for (size_t i = 0; i < this->holes[k]->size(); i++) {
			auto h = this->holes[k];
			all_vertexes.push_back(&((*h)[i]));
		}
	}
}

void PolygonTriangulator::set_trip_type(PolygonVertex& vertex) {

	auto pprev = vertex.prev().point;
	auto pnext = vertex.next().point;
	auto pme = vertex.point;
	int turn = left_turn(pprev, pme, pnext);
	// SPLIT
	if (pme.x < pprev.x && pme.x <= pnext.x && turn == -1) {
		vertex.type = TRIP_SPLIT;
		return;
	}
	// MERGE
	if (pprev.x < pme.x && pnext.x <= pme.x && turn == -1) {
		vertex.type = TRIP_MERGE;
		return;
	}
	// START
	if (pme.x <= pprev.x && pme.x < pnext.x && turn == 1) {
		vertex.type = TRIP_START;
		return;
	}
	// END
	if (pprev.x <= pme.x && pnext.x < pme.x && turn == 1) {
		vertex.type = TRIP_END;
		return;
	}
	// REGULAR (upper and lower)
	if (pprev.x <= pme.x && pme.x <= pnext.x) {
		vertex.type = TRIP_REGULAR;
		return;
	}
	if (pnext.x <= pme.x && pme.x <= pprev.x) {
		vertex.type = TRIP_REGULAR;
		return;
	}

	throw logic_error("Unknown vertex type case");

}

void PolygonTriangulator::fill_splits() {

	vector<PolygonVertex*> events(all_vertexes);
	Status status;

	DCEL dcel;

	sort(events.begin(), events.end(), [](PolygonVertex* v, PolygonVertex* u) {
		return *v < *u;
	});

	for (auto v : events)
		dcel.add_vertex(v);
	for (auto v : events)
		dcel.add_segment(v, &v->next());

	for (auto v : events) {
		auto type = v->type;
		auto helper = status.get_helper(v);
		if (type == TRIP_SPLIT) {
			splits.push_back(PolygonHoleSegment(*helper, *v));
		}
		if (helper != NULL && helper->type == TRIP_MERGE && type != TRIP_START
				&& type != TRIP_SPLIT // because it's repeat previous step
						) {
			splits.push_back(PolygonHoleSegment(*helper, *v));
		}
		if (type == TRIP_MERGE) {
			auto helperLower = status.get_lower_helper(v);
			if (helperLower->type == TRIP_MERGE) {
				splits.push_back(PolygonHoleSegment(*helperLower, *v));
			}
		}
		status.remove(v);
		status.add(v);
	}

	for (auto s : splits)
		dcel.add_segment(&s.a, &s.b);

	vector<PolygonVertex*> pvs;

	for (size_t i = 0; i < this->polygon.size(); i++) {
		pvs.push_back(&this->polygon[i]);
	}
	splitted_polygons = dcel.get_all_facets(pvs);

	// remove outer polygon (original polygon added reversed)
	for (auto it = splitted_polygons.begin(); it != splitted_polygons.end();
			++it) {
		const polygon_type& p = *it;
		auto itp0 = find(p.begin(), p.end(), polygon[1].point);
		if (itp0 == p.end())
			continue;
		itp0++;
		if (itp0 == p.end())
			itp0 = p.begin();
		if (*itp0 != polygon[0].point)
			continue;
		splitted_polygons.erase(it);
		break;
	}

}

void PolygonTriangulator::triangulate_monotonous(const polygon_type& polygon,
		vector<segment_type>& res) {

}

void PolygonTriangulator::triangulate() {
	for (auto pc : splitted_polygons) {
		cout << "polygon: ";
		for (auto p : pc) {
			cout << "(" << p.x << ", " << p.y << ") ";
		}
		cout << endl;
	}

}

PolygonTriangulator::~PolygonTriangulator() {
	for (auto h : holes) {
		delete h;
	}
}
