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
                
                point_type get_left_end(size_t pi) {
                    size_t prev = (pi - 1 + polygon.size()) % polygon.size();
                    size_t next = (pi + 1) % polygon.size();
                    if(polygon[prev].x < polygon[next].x)
                        return polygon[prev];
                    return polygon[next];
                }

                bool operator()(const size_t& a, const size_t& b) {
                    if(a == b) return false;
                    point_type pa(0,0);
                    point_type pb(0,0);
                    point_type pc(0,0);
                    point_type pd(0,0);
                    
                    if(a < polygon.size()) {
                        pa = get_left_end(a);
                        pb = polygon[a];
                    } else {
                        pa = polygon[a - polygon.size()];
                        pb = point_type(pa.x + 1, pa.y);
                    }
                    
                    if(b < polygon.size()) {
                        pc = get_left_end(b);
                        pd = polygon[b];
                    } else {
                        pc = polygon[b - polygon.size()];
                        pd = point_type(pc.x + 1, pc.y);
                    }
                    
                    if(pa == pc) {
                        if(pb.y > pd.y) return true;
                        return false;
                    }
                    
                    if(pb == pd) {
                        if(pa.y > pc.y) return true;
                        return false;
                    }
                    
                    // else vector product
                    int a1 = pb.x - pa.x;
                    int b1 = pb.y - pa.y;
                    
                    int a2 = pd.x - pc.x;
                    int b2 = pd.y - pc.y;
                    
                    int prod = a1 * b2 - b1 * a2;
                    
                    return prod < 0;
                    
                }
                
            };

        public:

            Status(const vector<point_type>& polygon) : polygon(polygon),
            segmentRightEndHelper(polygon) {
            }
            
            void add_segment(size_t from, size_t to) {
                // cout << "zero before add " << segmentRightEndHelper[(size_t)0]  << " -- " << to << endl;
                cout << "add (" << to << ", " << from << ") " << endl;
                segmentRightEndHelper[to] = from;
                // cout << "zero before add " << segmentRightEndHelper[(size_t)0]  << " -- " << to << endl;
            }
            
            void update_segment_helper(size_t to) {
                auto it = segmentRightEndHelper.upper_bound(to);
                cout << "update (" << it->first << ", " << it->second << ") -> ";
                it->second = to;
                cout << "(" << it->first << ", " << it->second << ")" << endl;
            }

            void remove_segment_with_end(size_t to) {
                cout << "remove " << to << endl;
                segmentRightEndHelper.erase(to);
            }

            size_t get_segment_helper(size_t to) {
                if(segmentRightEndHelper.find(to) != segmentRightEndHelper.end()){
                    return segmentRightEndHelper[to];
                }
                // compare as a point, comparator know about it
                return segmentRightEndHelper.lower_bound(to + polygon.size())->second;
            }
                        

        private:
            const vector<point_type>& polygon;
            map<size_t, size_t, PointComp> segmentRightEndHelper;

        };


    }
}

