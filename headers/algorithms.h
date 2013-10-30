#pragma once

#include "stdafx.h"
#include "geom/primitives/point.h"
#include "geom/primitives/vector.h"
#include "geom/primitives/segment.h"    

using namespace std;

namespace geom {
    namespace algorithms {
        
        using namespace geom::structures;
        
        // return true if polygon was oriented clockwise
        int left_turn(point_type a, point_type b, point_type c);

        int left_turn(const segment_type& segment, point_type p);

        bool point_on_line(const point_type& point, const segment_type& segment);

        bool segments_intersected(const segment_type& a, const segment_type& b);

        bool orient_polygon_anticlockwise(vector<point_type>& pts_);
        
        bool orient_polygon_clockwise(vector<point_type>& pts_);

        bool check_intersections(const vector<point_type>& pts_);
        
        void triangulate_monotonous(const vector<point_type>& pts_, 
                vector<segment_type>& res);
        
        bool is_point_strong_inside(const vector<point_type>& polygon,
                const point_type& point);
        
        bool is_polygon_inside(const vector<point_type>& polygonOuter, 
                const vector<point_type>& polygonIn);
    }
    
}

