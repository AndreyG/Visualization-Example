#include "triangulation_status.h"
#include "triangulation_types.h"
#include "geom/primitives/point.h"
#include "geom/primitives/segment.h"
#include "algorithms_geom.h"
#include <algorithm>

#include <vector>
#include <iostream>

using namespace std;
using namespace geom::structures;
using namespace geom::algorithms;

void Status::add(const PolygonVertex* v) {
	update_helper(v);
	auto rightEnd = get_right_end(v);
	if(rightEnd == NULL)
		return;
	size_t newSegmentIndex = segments.size();
	segments.push_back(PolygonHoleSegment(*v, *rightEnd));
	rightEndToSegment[rightEnd] = newSegmentIndex;
}

void Status::remove(const PolygonVertex* v) {
	auto it = rightEndToSegment.find(v);
	if(it == rightEndToSegment.end())
		return;
	helper.erase(helper.find(it->second));
}

PolygonVertex* Status::get_helper(const PolygonVertex* v) {
	return NULL;
}

void Status::update_helper(const PolygonVertex* v){
	
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



