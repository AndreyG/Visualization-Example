#pragma once

#include "geom/primitives/point.h"
#include "geom/primitives/vector.h"
#include "geom/primitives/segment.h"    
#include <vector>
#include <iostream>
#include <math.h>
#include <stack>

using namespace std;

namespace geom {
    namespace algorithms {
        
        // return true if polygon was oriented clockwise
        int left_turn(point_type a, point_type b, point_type c) {
            int det = a.x * (b.y - c.y) - a.y * (b.x - c.x) + (b.x * c.y - b.y * c.x);
            if (det > 0) return 1;
            if (det < 0) return -1;
            return 0;
        }

        int left_turn(const segment_type& segment, point_type p) {
            return left_turn(segment[0], segment[1], p);
        }

        bool point_on_line(const point_type& point, const segment_type& segment) {
            if (left_turn(segment, point) != 0) return false;
            if (point.x < min(segment[0].x, segment[1].x)) return false;
            if (point.x > max(segment[0].x, segment[1].x)) return false;
            if (point.y < min(segment[0].y, segment[1].y)) return false;
            if (point.y > max(segment[0].y, segment[1].y)) return false;
            return true;
        }

        bool segments_intersected(const segment_type& a, const segment_type& b) {

            if (max(a[0].x, a[1].x) < min(b[0].x, b[1].x))
                return false;
            if (max(b[0].x, b[1].x) < min(a[0].x, a[1].x))
                return false;
            if (max(a[0].y, a[1].y) < min(b[0].y, b[1].y))
                return false;
            if (max(b[0].y, b[1].y) < min(a[0].y, a[1].y))
                return false;
            
            int res1 = left_turn(a[0], a[1], b[0]);
            res1 *= left_turn(a[0], a[1], b[1]);

            int res2 = left_turn(b[0], b[1], a[0]);
            res2 *= left_turn(b[0], b[1], a[1]);
            
            return (res1 <= 0 && res2 <= 0);

        }

        bool orient_polygon_anticlockwise(vector<point_type>& pts_) {
            if (pts_.size() < 2) return false;
            auto minIt = min_element(pts_.begin(), pts_.end());
            size_t minI = minIt - pts_.begin();

            point_type left_most_v = pts_[minI];
            point_type next_v = pts_[(minI + 1) % pts_.size()];
            point_type prev_v = pts_[(minI - 1 + pts_.size()) % pts_.size()];

            bool isLeft = left_turn(prev_v, left_most_v, next_v) > 0;


            if (isLeft) return true;

            reverse(pts_.begin(), pts_.end());

            return false;
        }

        bool check_intersections(const vector<point_type>& pts_) {
            for (uint i = 0; i < pts_.size(); i++) {
                const segment_type segmentA(
                        pts_[i],
                        pts_[(i + 1) % pts_.size()]
                );
                for (uint t = 2; t <= pts_.size() - 2; t++) {
                    const segment_type segmentB(
                            pts_[(i + t) % pts_.size()],
                            pts_[(i + t + 1) % pts_.size()]
                    );
                    if (segments_intersected(segmentA, segmentB)){ 
                        cout << "Was intersection : " 
                                << segmentA[0] << segmentA[1] << " - "
                                << segmentB[0] << segmentB[1] << endl;
                        return true;
                    }
                }
                const segment_type segmentNext(
                        pts_[(i + 1) % pts_.size()],
                        pts_[(i + 2) % pts_.size()]
                        );
                if (point_on_line(segmentNext[1], segmentA) ||
                        point_on_line(segmentA[0], segmentNext)) {
                    return true;
                }
            }
            return false;
        }
        
        void triangulate_monotonous(const vector<point_type>& pts_, vector<segment_type>& res){
            
//            cout << "tri begin" << endl;
//            for(int i = 0 ; i < pts_.size(); i++){
//                cout << pts_[i] << " ";
//            }
//            cout << endl;
            
            vector<point_type> pts(pts_);
            cout << "points: ";
            for (uint i = 0; i < pts.size(); i++) {
                cout << pts[i] << " ";
            }
            cout << endl;
            auto minIt = min_element(pts.begin(), pts.end());
            
            cout << "min point: " << *minIt << endl;
            
            rotate(pts.begin(), minIt, pts.end());
            cout << "rotated points: " << endl;
            for(uint i = 0 ; i < pts.size(); i++){
                cout << pts[i] << " ";
            }
            cout << endl;
       
            size_t maxI = max_element(pts.begin(), pts.end()) - pts.begin();
            
            cout << "maxI: " << maxI << endl;
            
            size_t orderByXY[pts.size()];
            for(size_t i = 0; i < pts.size(); i++) orderByXY[i] = i;
            sort(orderByXY, orderByXY + pts.size(), [pts](size_t i, size_t j) {
                return pts[i] < pts[j];
            });
            
            cout << "order points: " << endl;
            for (int i = 0; i < pts.size(); i++) {
                cout << pts[orderByXY[i]] << " ";
            }
            cout << endl;
           
            stack<size_t> st;
            st.push(orderByXY[0]);
            st.push(orderByXY[1]);
            cout << "processing order: ";
            for(size_t i = 2; i < pts.size() - 1; i++) {
                bool prevWasTop = st.top() >= maxI;
                bool meTop = orderByXY[i] >= maxI;
                
                cout << orderByXY[i] << " ";
                
                if(prevWasTop != meTop){
                    auto mostTop = st.top();
                    while(!st.empty()){
                        auto topP = pts[mostTop];
                        st.pop();
                        res.push_back(segment_type(topP, pts[orderByXY[i]]));
                    }
                    st.push(mostTop);
                } else {
                    while(st.size() > 2){
                        auto topA = st.top();
                        st.pop();
                        auto topB = st.top();
                        if(left_turn(pts[orderByXY[i]], pts[topA], pts[topB]) == -1){
                            res.push_back(segment_type(pts[topB], pts[orderByXY[i]]));
                        } else {
                            st.push(topA);
                            break;
                        }
                    }
                }
                
                st.push(orderByXY[i]);
                
//                res.push_back(segment_type(pts[orderByXY[i]], pts[orderByXY[i-1]]));
            }
            
            cout << endl;
            
        }

    }
}

