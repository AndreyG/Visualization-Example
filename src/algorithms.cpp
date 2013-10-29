
#include "algorithms.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <stack>

#include "io/point.h"



namespace geom {
    namespace algorithms {
        
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
            if (point.x < std::min(segment[0].x, segment[1].x)) return false;
            if (point.x > std::max(segment[0].x, segment[1].x)) return false;
            if (point.y < std::min(segment[0].y, segment[1].y)) return false;
            if (point.y > std::max(segment[0].y, segment[1].y)) return false;
            return true;
        }

        bool segments_intersected(const segment_type& a, const segment_type& b) {

            if (std::max(a[0].x, a[1].x) < std::min(b[0].x, b[1].x))
                return false;
            if (std::max(b[0].x, b[1].x) < std::min(a[0].x, a[1].x))
                return false;
            if (std::max(a[0].y, a[1].y) < std::min(b[0].y, b[1].y))
                return false;
            if (std::max(b[0].y, b[1].y) < std::min(a[0].y, a[1].y))
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
                    if (segments_intersected(segmentA, segmentB)) {
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

        void triangulate_monotonous(const vector<point_type>& pts_, vector<segment_type>& res) {

            vector<point_type> pts(pts_);
            auto minIt = min_element(pts.begin(), pts.end());

            rotate(pts.begin(), minIt, pts.end());

            size_t maxI = max_element(pts.begin(), pts.end()) - pts.begin();

            size_t orderByXY[pts.size()];
            for (size_t i = 0; i < pts.size(); i++) orderByXY[i] = i;
            sort(orderByXY, orderByXY + pts.size(), [pts, maxI](size_t i, size_t j) {
                if (pts[i].x == pts[j].x) {
                    bool onTopI = i >= maxI;
                    bool onTopJ = j >= maxI;
                    if (onTopI != onTopJ)
                        return onTopI ? true : false;
                    if (onTopI) return i > j;
                    return i < j;
                }
                return pts[i].x < pts[j].x;
            });

            stack<size_t> st;
            st.push(orderByXY[0]);
            st.push(orderByXY[1]);

            for (size_t i = 2; i < pts.size() - 1; i++) {
                bool prevWasTop = st.top() >= maxI;
                bool meTop = orderByXY[i] >= maxI;
                auto curPoint = pts[orderByXY[i]];

                if (prevWasTop != meTop) {
                    auto newTop = st.top();
                    while (st.size() > 1) {
                        res.push_back(segment_type(pts[st.top()], curPoint));
                        st.pop();
                    }
                    st.pop();
                    st.push(newTop);
                } else {
                    int currentTurn = meTop ? 1 : -1;
                    while (st.size() > 1) {
                        auto bi = st.top();
                        st.pop();
                        auto ci = st.top();
                        bool isRightTurn = left_turn(curPoint, pts[bi], pts[ci]) == currentTurn;
                        if (isRightTurn) {
                            res.push_back(segment_type(curPoint, pts[ci]));
                        } else {
                            st.push(bi);
                            break;
                        }
                    }
                }

                st.push(orderByXY[i]);

            }

            st.pop();
            while (st.size() > 1) {
                res.push_back(segment_type(pts[st.top()], pts[maxI]));
                st.pop();
            }


        }

    }
}

