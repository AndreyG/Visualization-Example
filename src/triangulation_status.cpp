#include "triangulation_status.h"
#include "triangulation_types.h"
#include "geom/primitives/point.h"
#include "geom/primitives/segment.h"
#include "algorithms_geom.h"
#include <algorithm>

#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace geom::structures;
using namespace geom::algorithms;

void Status::add(const PolygonVertex* v) {
	update_helper(v);
	auto rightEnd = get_right_end(v);
	if (rightEnd == NULL)
		return;
	size_t newSegmentIndex = segments.size();
	segments.push_back(PolygonHoleSegment(*v, *rightEnd));
	rightEndToSegment[rightEnd] = newSegmentIndex;
	helper[newSegmentIndex] = NULL;
}

void Status::remove(const PolygonVertex* v) {
	auto it = rightEndToSegment.find(v);
	if (it == rightEndToSegment.end())
		return;
	helper.erase(helper.find(it->second));
}

const PolygonVertex* Status::get_helper(const PolygonVertex* v) {
	auto it = rightEndToSegment.find(v);
	if (it != rightEndToSegment.end()) {
		return helper[it->second]; // sometimes it is NULL
	}
	PolygonHoleSegment seg(*v, *v);
	segments.push_back(seg);
	auto ith = helper.upper_bound(segments.size() - 1);
	segments.pop_back();

	if (ith == helper.end())
		return NULL;
	if (ith->second != NULL)
		return ith->second;
	return &segments[ith->first].a;

}

void Status::update_helper(const PolygonVertex* v) {
	PolygonHoleSegment seg(*v, *v);
	segments.push_back(seg);
	auto it = helper.upper_bound(segments.size() - 1);
	segments.pop_back();
	if (it == helper.end())
		return;
	it->second = v;
}

const PolygonVertex* Status::get_right_end(const PolygonVertex* v) {
	if (v->type == TRIP_END)
		return NULL;
	if (v->type == TRIP_MERGE)
		return NULL;
	if (v->type == TRIP_START)
		return &v->next();
	if (v->type == TRIP_SPLIT)
		return &v->next();
	if (v->is_lower_regular())
		return &v->next();
	return NULL;
}

bool Status::HelperComparator::operator()(const size_t& a, const size_t b) {

	if (a == b)
		return false;

	auto sa = this->segments[a].get_segment();
	auto sb = this->segments[b].get_segment();

	if (max(sb[0].y, sb[1].y) < min(sa[0].y, sa[1].y))
		return true;

	if (max(sa[0].y, sa[1].y) < min(sb[0].y, sb[1].y))
		return false;

	int turnsa0 = left_turn(sa, sb[0]);
	int turnsa1 = left_turn(sa, sb[1]);
	if (turnsa0 == turnsa1 || turnsa0 == 0) {
		return turnsa1 == -1; // because we need not less but great
	}

	int turnsb0 = left_turn(sb, sa[0]);
	int turnsb1 = left_turn(sb, sa[1]);
	if (turnsb0 != turnsb1 || turnsb0 == 0) {
		throw std::logic_error("Segments intersected!");
	} else {
		return turnsb1 == 1;
	}

}

