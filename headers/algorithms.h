#pragma once

#include "geom/primitives/point.h"
#include "geom/primitives/vector.h"
#include <vector>
#include <iostream>

namespace geom {
    namespace algorithms {
        // return true if polygon was oriented clockwise

        int left_turn(point_type a, point_type b, point_type c) {
            int det = a.x * (b.y - c.y) - a.y * (b.x - c.x) +
                (b.x * c.y - b.y * c.x);
            if (det > 0) return 1;
            if (det < 0) return -1;
            return 0;
        }

        bool orient_polygon_anticlockwise(std::vector<point_type>& pts_) {
            if (pts_.size() < 2) return false;
            auto minIt = std::min_element(pts_.begin(), pts_.end(),
                    [](point_type a, point_type b) {
                        return a.x < b.x; });
            size_t minI = minIt - pts_.begin();

            point_type left_most_v = pts_[minI] + 
                    geom::structures::vector_type(-1, 0);
            point_type next_v = pts_[(minI + 1) % pts_.size()];
            point_type prev_v = pts_[(minI - 1 + pts_.size()) % pts_.size()];
             
            bool isLeft = left_turn(prev_v, left_most_v, next_v) > 0;
            
            std::cout << isLeft << std::endl;
            
            if (isLeft) return true;

            std::reverse(pts_.begin(), pts_.end());
            
            return false;
        }

    }
}

