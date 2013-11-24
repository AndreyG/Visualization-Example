#pragma once 

#include "algorithm"
#include "algorithms_geom.h"
#include "vector"

using namespace std;
using namespace geom::algorithms;

enum TRIP_TYPE {
	TRIP_SPLIT, TRIP_MERGE, TRIP_START, TRIP_END, TRIP_REGULAR
};

struct PolygonVertex {
	int contourI;  // -1 - in polygon. 0, 1, 2, .. - a hole index
	size_t index;
	TRIP_TYPE type;
	point_type point;
};

struct PolygonHoleSegment {
	PolygonVertex a;
	PolygonVertex b;
};

class PolygonTriangulator {

public:

	const polygon_type& polygon;
	const vector<polygon_type>& holes;
	vector<PolygonHoleSegment> splits;

	PolygonTriangulator(const polygon_type& polygon,
			const vector<polygon_type>& holes) :
			polygon(polygon), holes(holes) {
	}

	void triangulate() {
//		auto diagonals = get_tri_split(polygon);
	}
	
	vector<pair<point_type, TRIP_TYPE> > get_points_types();
	vector<pair<point_type, TRIP_TYPE> > get_split_segments();

private:
	static void triangulate_monotonous(const polygon_type& polygon,
			vector<segment_type>& res);
	void fill_points_types();								// get_points_types
	void fill_splits(); 	// get_tri_split
	void set_trip_type(PolygonVertex& vertex); 				// get_trip_type 

};
