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
        
        inline size_t next(const vector<point_type>& polygon, size_t i){
            return (i + 1) % polygon.size();
        }
        
        inline point_type nextp(const vector<point_type>& polygon, size_t i){
            return polygon[next(polygon, i)];
        }
        
        inline size_t prev(const vector<point_type>& polygon, size_t i){
            return (i - 1 + polygon.size()) %  polygon.size();
        }
        
        inline point_type prevp(const vector<point_type>& polygon, size_t i){
            return polygon[prev(polygon, i)];
        }

        class Status {

          
        public:

            Status(const vector<point_type>& polygon) : polygon(polygon) {
            }
            
            void add_segment(size_t i) {
                TRIP_TYPE type = get_trip_type(polygon, i, false);
                if(type == TRIP_START || type == TRIP_SPLIT){
                    insert(next(polygon, i));
                }
                if(type == TRIP_REGULAR){
                    if(polygon[i].x < nextp(polygon, i).x)
                        insert(next(polygon, i));
                }
                
                
            }
            
            void insert(size_t i){
                point_type leftEnd = prevp(polygon, i);
                auto it = segments.begin();
                while(it != segments.end()){
                    auto pa = prevp(polygon, *it);
                    auto pb = polygon[*it];
                    int turn = left_turn(segment_type(pa, pb), leftEnd);
                    if(turn < 0) break;
                    it++;
                }
                segments.insert(it, i);
                helper[i] = prev(polygon, i);
            }
            
            void update_segment_helper(size_t i) {
                TRIP_TYPE type = get_trip_type(polygon, i, false);
                if(type == TRIP_REGULAR && prevp(polygon, i).x <= polygon[i].x)
                    return;
                if(type == TRIP_START)
                    return;
                if(type == TRIP_END)
                    return;
                
                size_t lastPrev = polygon.size();
                point_type pnt = polygon[i];
                for(auto seg : segments) {
                    auto pa = prevp(polygon, seg);
                    auto pb = polygon[seg];
                    int turn = left_turn(segment_type(pa, pb), pnt);
                    if(turn > 1) lastPrev = seg;
                    else break;
                }
                if(lastPrev == polygon.size()){
                    cout << "NOT FOUND LOWER SEGMENT FOR " << i << " !" << endl;
                    return;
                }
                helper[lastPrev] = i;
            }
            

            void remove_segment_with_end(size_t to) {
                auto it = find(segments.begin(), segments.end(), to);
                if(it == segments.end()) {
                    cout << "NOT FOUND SEGMENT END " << to << " !" << endl;
                    return;
                }
                segments.erase(it);
            }

            size_t get_segment_helper(size_t to) {
                auto it = helper.find(to);
                if(it == helper.end()) {
                    cout << "NOT FOUND HELPER FOR " << to << " !" << endl;
                    return 0;
                }
                return helper[to];
            }
                        

        private:
            const vector<point_type>& polygon;
            map<size_t, size_t> helper;
            vector<size_t> segments;

        };


    }
}

