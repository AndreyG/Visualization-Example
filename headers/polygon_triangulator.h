#pragma once 

#include "triangulation_types.h"

#include "algorithm"
#include "algorithms_geom.h"
#include "vector"

using namespace std;
using namespace geom::algorithms;

class PolygonTriangulator {

public:

	const TriPolygon& polygon;
	const vector<TriPolygon>& holes;
	vector<PolygonHoleSegment> splits;

	PolygonTriangulator(const polygon_type& polygon,
			const vector<polygon_type>& holes) :
			polygon(polygon), holes(holes) {
	}

	void triangulate() {
//		auto diagonals = get_tri_split(polygon);
	}

	vector<pair<point_type, TRIP_TYPE> > get_points_types();
	vector<segment_type> get_split_segments();

private:
	static void triangulate_monotonous(const polygon_type& polygon,
			vector<segment_type>& res);
	void fill_points_types();
	void fill_splits();
	void set_trip_type(PolygonVertex& vertex);

};
