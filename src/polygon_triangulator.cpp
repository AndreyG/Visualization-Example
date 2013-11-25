#include "polygon_triangulator.h"
#include "status.h"
#include <algorithm>
#include <vector>


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

	if (pprev.x < pme.x && pme.x < pnext.x) {
		vertex.type = TRIP_REGULAR;
		return;
	}

	if (pme.x <= pprev.x && pme.x < pnext.x) {
		auto turn = left_turn(pprev, pme, pnext);
		if (turn > 0) {
			vertex.type = TRIP_START;
			return;
		}
	}

	if (pme.x < pprev.x && pme.x < pnext.x) {
		vertex.type = TRIP_SPLIT;
		return;
	}

	if (pme.x > pprev.x && pme.x >= pnext.x) {
		auto turn = left_turn(pprev, pme, pnext);
		if (turn > 0) {
			vertex.type = TRIP_END;
			return;
		} else {
			vertex.type = TRIP_MERGE;
			return;
		}
	}

	vertex.type = TRIP_REGULAR;

}

void PolygonTriangulator::fill_splits() {

	vector<PolygonVertex*> orderByXY(all_vertexes);
	Status status;
	
	sort(orderByXY.begin(), orderByXY.end(), [](PolygonVertex* i, PolygonVertex* j) {
		return i->point < j->point;
	});

	for (auto vp : orderByXY) {

		auto type = vp->type;
		if (type == TRIP_START) {
			status.add(*vp);
			continue;
		}

		bool foundGoodHelper = false;
		PolygonVertex* helper = status.helper(*vp);
		if (helper != NULL && *helper != vp->prev()) {
			foundGoodHelper = true;
		}
		if (!foundGoodHelper) {
			PolygonVertex* lowSevp = status.find_lower_segment(*vp);
			if (lowSevp != NULL) {
				helper = status.helper(*lowSevp);
				foundGoodHelper = true;
			}
		}
		if (foundGoodHelper) {
			TRIP_TYPE helperType = helper->type;
//			cout << "(" << i << ", " << helper << ") " << endl;
			if (helperType == TRIP_MERGE) {
				splits.push_back(PolygonHoleSegment(*vp, *helper));
			}
			if (type == TRIP_SPLIT) {
				splits.push_back(PolygonHoleSegment(*vp, *helper));
			}
		}
		status.remove(*vp);
		status.update(*vp);
		status.add(*vp);

	}

}

void PolygonTriangulator::triangulate_monotonous(const polygon_type& polygon,
		vector<segment_type>& res) {
}

void PolygonTriangulator::triangulate() {
}

PolygonTriangulator::~PolygonTriangulator() {
	for (auto h : holes) {
		delete h;
	}
}
