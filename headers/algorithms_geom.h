#pragma once

#include "stdafx.h"
#include "geom/primitives/point.h"
#include "geom/primitives/vector.h"
#include "geom/primitives/segment.h"
#include <set>
#include <map>

using namespace std;
using namespace geom::structures;

typedef std::vector<point_type> polygon_type;


namespace geom {
namespace algorithms {

using namespace geom::structures;



void triangulate(const polygon_type& polygon, const vector<polygon_type>& holes,
		vector<segment_type>& res);

int left_turn(point_type a, point_type b, point_type c);

int left_turn(const segment_type& segment, point_type p);

bool point_on_line(const point_type& point, const segment_type& segment);

bool segments_intersected(const segment_type& a, const segment_type& b);

// return true if polygon was oriented clockwise
bool orient_polygon_anticlockwise(vector<point_type>& pts_);

bool orient_polygon_clockwise(polygon_type& pts_);

bool check_intersections(const polygon_type& pts_);

bool is_point_strong_inside(const polygon_type& polygon,
		const point_type& point);

bool is_polygon_inside(const polygon_type& polygonOuter,
		const polygon_type& polygonIn);

bool is_polygons_intersected(const vector<polygon_type>& polygonA,
		const polygon_type& polygonB);

inline size_t next(const polygon_type& polygon, size_t i) {
	return (i + 1) % polygon.size();
}

inline point_type nextp(const polygon_type& polygon, size_t i) {
	return polygon[next(polygon, i)];
}

inline size_t prev(const polygon_type& polygon, size_t i) {
	return ((i + polygon.size()) - 1) % polygon.size();
}

inline point_type prevp(const polygon_type& polygon, size_t i) {
	return polygon[prev(polygon, i)];
}

}
}

