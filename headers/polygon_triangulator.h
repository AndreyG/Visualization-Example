#pragma once 

#include "triangulation_types.h"

#include "algorithm"
#include "algorithms_geom.h"
#include "vector"

using namespace std;
using namespace geom::algorithms;

class PolygonTriangulator {

	const TriPolygon polygon;
	vector<TriPolygon> holes;
	vector<PolygonHoleSegment> splits;
	vector<PolygonHoleSegment> triangulation_segments;

public:

	PolygonTriangulator(const polygon_type& polygon,
			const vector<polygon_type>& holes) :
			polygon(polygon) {

		for (auto h : holes) {
			this->holes.push_back(TriPolygon(h));
		}
		fill_points_types();
		fill_splits();
		triangulate();
	}

	vector<pair<point_type, TRIP_TYPE> > get_points_types();
	vector<segment_type> get_split_segments();
	vector<segment_type> get_triangulation_segments();

private:

	void fill_points_types();
	void fill_splits();
	void set_trip_type(PolygonVertex& vertex);
	static void triangulate_monotonous(const polygon_type& polygon,
			vector<segment_type>& res);
	void triangulate();

};
