#pragma once

#include "visualization/viewer_adapter.h"
#include "visualization/draw_util.h"
#include "io/point.h"
#include "polygon_triangulator.h"

#include "algorithms_geom.h"
#include <iostream>


using namespace visualization;
using geom::structures::point_type;
using geom::structures::contour_type;

struct app_viewer : viewer_adapter {
    void draw(drawer_type & drawer) const;
    void print(printer_type & printer) const;
    point_type get_info_pnt() const;
    point_type get_error_pnt() const;
    point_type get_legend_pnt() const;

    bool on_double_click(point_type const & pt);
    bool on_key(int key);

    bool on_polygon_drawing_start();
    bool on_polygon_drawing_stop();
    bool on_hole_drawing_start();
    bool on_hole_drawing_stop();
    bool on_polygon_drawing_click(point_type const & pt);
    
    bool on_triangulate();

    void restore_init_state();
    
    bool save_state();
    bool load_state();
    
private:
    polygon_type polygon;
    std::vector<point_type> cur_drawing_pts;   // polygon
    std::vector<segment_type> tri_segms;       // result after triangulation
    std::vector<polygon_type> holes;
    std::vector<std::pair<point_type, PolygonTriangulator::TRIP_TYPE> > point_marks;
    std::vector<pair<size_t, size_t> > split_diagonals;
    std::string error_str = "";
    bool is_polygon_draw_state = false;
    bool is_polygon_loaded_successfully = false;
    bool is_hole_draw_state = false;
};

