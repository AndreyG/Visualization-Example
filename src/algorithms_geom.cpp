#include "algorithms_geom.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <stack>
#include "polygon_triangulator.h"
#include "io/point.h"
#include <iostream>

using namespace std;

namespace geom {
namespace algorithms {

void triangulate(const polygon_type& polygon, const vector<polygon_type>& holes,
		vector<segment_type>& res) {
	PolygonTriangulator tr(polygon, holes);
	tr.triangulate();
}

int left_turn(point_type a, point_type b, point_type c) {
	int det = a.x * (b.y - c.y) - a.y * (b.x - c.x) + (b.x * c.y - b.y * c.x);
	if (det > 0)
		return 1;
	if (det < 0)
		return -1;
	return 0;
}

int left_turn(const segment_type& segment, point_type p) {
	return left_turn(segment[0], segment[1], p);
}

bool point_on_line(const point_type& point, const segment_type& segment) {
	if (left_turn(segment, point) != 0)
		return false;
	if (point.x < std::min(segment[0].x, segment[1].x))
		return false;
	if (point.x > std::max(segment[0].x, segment[1].x))
		return false;
	if (point.y < std::min(segment[0].y, segment[1].y))
		return false;
	if (point.y > std::max(segment[0].y, segment[1].y))
		return false;
	return true;
}

bool segments_intersected(const segment_type& a, const segment_type& b) {
	if (std::max(a[0].x, a[1].x) < std::min(b[0].x, b[1].x))
		return false;
	if (std::max(b[0].x, b[1].x) < std::min(a[0].x, a[1].x))
		return false;
	if (std::max(a[0].y, a[1].y) < std::min(b[0].y, b[1].y))
		return false;
	if (std::max(b[0].y, b[1].y) < std::min(a[0].y, a[1].y))
		return false;

	int res1 = left_turn(a[0], a[1], b[0]);
	res1 *= left_turn(a[0], a[1], b[1]);

	int res2 = left_turn(b[0], b[1], a[0]);
	res2 *= left_turn(b[0], b[1], a[1]);

	return (res1 <= 0 && res2 <= 0);
}

bool orient_polygon_anticlockwise(vector<point_type>& pts_) {
	if (pts_.size() < 2)
		return false;
	auto minIt = min_element(pts_.begin(), pts_.end());
	size_t minI = minIt - pts_.begin();

	point_type left_most_v = pts_[minI];
	point_type next_v = pts_[(minI + 1) % pts_.size()];
	point_type prev_v = pts_[(minI - 1 + pts_.size()) % pts_.size()];

	bool isLeft = left_turn(prev_v, left_most_v, next_v) > 0;

	if (isLeft)
		return true;

	reverse(pts_.begin(), pts_.end());

	return false;
}

bool orient_polygon_clockwise(vector<point_type>& pts_) {
	bool res = orient_polygon_anticlockwise(pts_);
	reverse(pts_.begin(), pts_.end());
	return !res;
}

bool check_intersections(const vector<point_type>& pts_) {
	for (uint i = 0; i < pts_.size(); i++) {
		const segment_type segmentA(pts_[i], pts_[(i + 1) % pts_.size()]);
		for (uint t = 2; t <= pts_.size() - 2; t++) {
			const segment_type segmentB(pts_[(i + t) % pts_.size()],
					pts_[(i + t + 1) % pts_.size()]);
			if (segments_intersected(segmentA, segmentB)) {
				cout << "Was intersection : " << segmentA[0] << segmentA[1]
						<< " - " << segmentB[0] << segmentB[1] << endl;
				return true;
			}
		}
		const segment_type segmentNext(pts_[(i + 1) % pts_.size()],
				pts_[(i + 2) % pts_.size()]);
		if (point_on_line(segmentNext[1], segmentA)
				|| point_on_line(segmentA[0], segmentNext)) {
			return true;
		}
	}
	return false;
}

bool is_point_inside(const vector<point_type>& polygon,
		const point_type& point) {
	int32 minx = min_element(polygon.begin(), polygon.end())->x;
	segment_type ray(point_type(minx - 1, point.y + 1), point);
	bool inside = false;
	for (size_t i = 0; i < polygon.size(); i++) {
		point_type from = polygon[i];
		point_type to = polygon[(i + 1) % polygon.size()];
		if (segments_intersected(segment_type(from, to), ray))
			inside = !inside;
	}
	return inside;
}

bool is_polygon_inside(const vector<point_type>& polygonOuter,
		const vector<point_type>& polygonIn) {
	for (auto p : polygonIn) {
		if (!is_point_inside(polygonOuter, p))
			return false;
	}
	return true;
}

bool is_polygons_intersected(const polygon_type& polygonA,
		const polygon_type& polygonB) {
	for (size_t i = 0; i < polygonA.size(); i++) {
		point_type froma = polygonA[i];
		point_type toa = polygonA[(i + 1) % polygonA.size()];
		segment_type sa(froma, toa);
		for (size_t j = 0; j < polygonB.size(); j++) {
			point_type fromb = polygonB[j];
			point_type tob = polygonB[(j + 1) % polygonB.size()];
			segment_type sb(fromb, tob);
			if (segments_intersected(sa, sb)){
				cout << sa[0] << " " << sa[1] << "-"  << sb[0] << " " << sb[1];
				return true;
			}
		}
	}
	return false;
}

bool is_polygons_intersected(const vector<polygon_type>& polygons,
		const vector<point_type>& polygon) {
	for(int i = 0; i < polygons.size(); i++){
		auto p = polygons[i];
		if (is_polygons_intersected(p, polygon)){
			cout << "found intersection with: " << i << endl;
			return true;
		}
	}
	return false;
}

}
}

