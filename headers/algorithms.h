#pragma once

#include "stdafx.h"
#include "geom/primitives/point.h"
#include "geom/primitives/vector.h"
#include "geom/primitives/segment.h"    
#include <set>
#include <map>

using namespace std;

namespace geom {
    namespace algorithms {

        using namespace geom::structures;

        enum TRIP_TYPE {
            TRIP_SPLIT,
            TRIP_MERGE,
            TRIP_START,
            TRIP_END,
            TRIP_REGULAR
        };

        int left_turn(point_type a, point_type b, point_type c);

        int left_turn(const segment_type& segment, point_type p);

        bool point_on_line(const point_type& point, const segment_type& segment);

        bool segments_intersected(const segment_type& a, const segment_type& b);

        // return true if polygon was oriented clockwise
        bool orient_polygon_anticlockwise(vector<point_type>& pts_);

        bool orient_polygon_clockwise(vector<point_type>& pts_);

        bool check_intersections(const vector<point_type>& pts_);

        void triangulate_monotonous(const vector<point_type>& pts_,
                vector<segment_type>& res);

        bool is_point_strong_inside(const vector<point_type>& polygon,
                const point_type& point);

        bool is_polygon_inside(const vector<point_type>& polygonOuter,
                const vector<point_type>& polygonIn);
        bool is_polygons_intersected(const vector<point_type>& polygonA,
                const vector<point_type>& polygonB);
        bool is_polygons_intersected(const vector<vector<point_type> >& polygons,
                const vector<point_type>& polygon);
        vector<pair<size_t, size_t> > get_tri_split(const vector<point_type>& polygon);

        vector<pair<point_type, TRIP_TYPE> > get_points_types(
                const vector<point_type>& polygon,
                const vector<vector<point_type> >& holes);

        TRIP_TYPE get_trip_type(const vector<point_type>& polygon, size_t index,
                bool isInHole);

        class Status {

            struct PointComp {
                const vector<point_type> & polygon;

                PointComp(const vector<point_type>& polygon) : polygon(polygon) {
                }

                bool operator()(const size_t& a, const size_t& b) {
                    return polygon[a].y > polygon[b].y;
                }
            };

        public:

            Status(const vector<point_type>& polygon) : polygon(polygon),
            segmentRightEndHelper(polygon) {
            }


            // possible to add start "segment" where from == to

            void add_segment(size_t from, size_t to) {
                segmentRightEndHelper[to] = from;
            }

            void update_segment_helper(size_t to) {
                segmentRightEndHelper.upper_bound(to)->second = to;
            }

            void remove_segment_with_end(size_t to) {
                segmentRightEndHelper.erase(to);
            }

            size_t get_segment_helper(size_t to) {
                return segmentRightEndHelper.upper_bound(to)->second;
            }

        private:
            const vector<point_type>& polygon;
            map<size_t, size_t, PointComp> segmentRightEndHelper;

        };


    }

}

