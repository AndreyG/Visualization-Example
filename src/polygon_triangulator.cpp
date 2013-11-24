#include "polygon_triangulator.h"
#include <algorithm>
#include <vector>

using namespace std;

vector<pair<point_type, TRIP_TYPE> > PolygonTriangulator::get_points_types() {
	vector<pair<point_type, TRIP_TYPE> > res;
	for (size_t i = 0; i < polygon.size(); i++) {
		res.push_back(make_pair(polygon[i].point, polygon[i].type));
	}

	for (size_t k = 0; k < holes.size(); k++) {
		for (size_t i = 0; i < holes[k].size(); i++) {
			res.push_back(make_pair(holes[k][i].point, holes[k][i].type));
		}
	}

	return res;
}

vector<segment_type> PolygonTriangulator::get_split_segments() {
	vector<segment_type> res;
	for (size_t i = 0; i < splits.size(); i++) {
		res.push_back(splits[i].get_segment());
	}
	return res;
}

vector<segment_type> PolygonTriangulator::get_triangulation_segments() {
	vector<segment_type> res;
	for (size_t i = 0; i < triangulation_segments.size(); i++) {
		res.push_back(triangulation_segments[i].get_segment());
	}
	return res;
}

void PolygonTriangulator::fill_points_types() {
	
}

void PolygonTriangulator::fill_splits() {
}

void PolygonTriangulator::set_trip_type(PolygonVertex& vertex) {
}

void PolygonTriangulator::triangulate_monotonous(const polygon_type& polygon,
		vector<segment_type>& res) {
}

void PolygonTriangulator::triangulate() {
}
