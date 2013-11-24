#pragma once 

#include "algorithm"
#include "algorithms_geom.h"
#include "vector"

using namespace std;
using namespace geom::algorithms;

class PolygonTriangulator {

	const polygon_type& polygon;
	const vector<polygon_type>& holes;

public:
	PolygonTriangulator(const polygon_type& polygon,
			const vector<polygon_type>& holes) :
			polygon(polygon), holes(holes) {
	}

	enum TRIP_TYPE {
		TRIP_SPLIT, TRIP_MERGE, TRIP_START, TRIP_END, TRIP_REGULAR
	};

	struct PolygonIndex {
		int contourI;  // -1 - in polygon. 0, 1, 2, .. - a hole index
		size_t index;
	};

	struct PolygonHoleSegment {
		PolygonIndex a;
		PolygonIndex b;
	};

	void triangulate() {
//		auto diagonals = get_tri_split(polygon);
	}

};
