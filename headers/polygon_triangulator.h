#pragma once 

#include "triangulation_types.h"

#include "algorithm"
#include "algorithms_geom.h"
#include "vector"
#include <functional>
#include <iostream>

using namespace std;
using namespace geom::algorithms;

class PolygonTriangulator {

	TriPolygon polygon;
	vector<TriPolygon*> holes;
	vector<PolygonHoleSegment> splits;
	vector<PolygonHoleSegment> triangulation_segments;
	vector<PolygonVertex*> all_vertexes;

public:

	PolygonTriangulator(const polygon_type& polygon_,
			const vector<polygon_type>& holes_) :
			polygon(polygon_, false) {

		for (auto h : holes_) {
			this->holes.push_back(new TriPolygon(h, true));
		}

		collect_all_vertexes();
		fill_points_types();
		fill_splits();
		triangulate();

	}

	vector<pair<point_type, TRIP_TYPE> > get_points_types();
	vector<segment_type> get_split_segments();
	vector<segment_type> get_triangulation_segments();
	
	~PolygonTriangulator();

private:

	void collect_all_vertexes();

	void fill_points_types();
	void set_trip_type(PolygonVertex& vertex);
	void fill_splits();
	static void triangulate_monotonous(const polygon_type& polygon,
			vector<segment_type>& res);
	void triangulate();

};
