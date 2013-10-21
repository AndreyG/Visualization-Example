#pragma once
#include "geom/primitives/segment.h"
#include "geom/primitives/vector.h"
#include "visualization/viewer.h"

#include "math.h"

using namespace geom::structures;
using namespace visualization;

namespace drawer {

    void drawArrow(drawer_type& drawer, const segment_type& segment,
            double width = 1.0) {

        // base line
        drawer.draw_line(segment, width);

        vector_type vt = segment[0] - segment[1];

        double angle = atan2(vt.y * 1.0, vt.x * 1.0);

        int xe = segment[1].x;
        int ye = segment[1].y;

        for (float i = -0.5f; i <= 0.5l; i += 0.1) {
            int xa = cos(angle + i) * 7 + xe;
            int ya = sin(angle + i) * 7 + ye;
            drawer.draw_line(segment_type(
                    point_type(xe, ye),
                    point_type(xa, ya)),
                    width);
        }

    }

}

