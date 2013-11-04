#include "app_viewer.h"
#include "stdafx.h"
#include "drawer.h"
#include "algorithms.h"

void app_viewer::draw(drawer_type & drawer) const {
    drawer.set_color(Qt::blue);
    for (point_type const & pt : cur_drawing_pts)
        drawer.draw_point(pt, 3);

    if (is_hole_draw_state)
        drawer.set_color(Qt::blue);
    else
        drawer.set_color(Qt::red);

    for (uint i = 1; i < cur_drawing_pts.size(); i++) {
        drawer.draw_line(segment_type(cur_drawing_pts[i - 1], cur_drawing_pts[i]));
    }

    drawer.set_color(Qt::red);
    drawer::drawPolygon(drawer, polygon);

    drawer.set_color(Qt::blue);
    for (polygon_type h : holes)
        drawer::drawPolygon(drawer, h);

    drawer.set_color(Qt::yellow);
    for (segment_type sg : tri_segms) {
        drawer.draw_line(sg);
    }

    for (auto p : point_marks) {
        drawer::drawTripVertex(drawer, p.first, p.second);
    }
    
    drawer.set_color(Qt::darkYellow);
    for(auto p : split_diagonals){
        drawer.draw_line(segment_type(polygon[p.first], polygon[p.second]));
    }
        
//    drawer::drawLegend(drawer, get_legend_pnt());
    
}

point_type app_viewer::get_legend_pnt() const {
    
    return point_type(
            -get_wnd()->width() / 2 + 10,
            -get_wnd()->height() / 2 + 30
            );
    
}

point_type app_viewer::get_info_pnt() const {
    return point_type(
            -get_wnd()->width() / 2 + 10,
            -get_wnd()->height() / 2 + 10
            );
}

point_type app_viewer::get_error_pnt() const {
    return point_type(
            -get_wnd()->width() / 2 + 10,
            -get_wnd()->height() / 2 + 50
            );
}

void app_viewer::print(printer_type & printer) const {
    printer.corner_stream() << "Points num: " << cur_drawing_pts.size() << endl;
    stream_type& global_stream = printer.global_stream(get_error_pnt());
    if (error_str != "")
        global_stream << "ERROR: " << error_str;

    stream_type& global_stream2 = printer.global_stream(get_info_pnt());

    if (is_polygon_draw_state) {
        global_stream2 << "Press SPACE button to STOP polygon drawing (last face will be added automatically)";
    } else {
        global_stream2 << "Press SPACE button to START polygon drawing (remove previous).";
        global_stream2 << " Or press H to start drawing a hole.";
    }
    
   stream_type& global_stream3 = printer.global_stream(get_legend_pnt());
   global_stream3 << "LEGEND:";
   global_stream3 << "         regular";
   global_stream3 << "         start";
   global_stream3 << "         split";
   global_stream3 << "         end";
   global_stream3 << "         merge";
    
}

bool app_viewer::on_double_click(point_type const & pt) {
    if (is_polygon_draw_state) {
        return on_polygon_drawing_click(pt);
    }
    return false;
}

bool app_viewer::on_key(int key) {
    switch (key) {
        case Qt::Key_Escape:
            get_wnd()->close();
            break;
        case Qt::Key_Space:
            if (!is_polygon_draw_state) return on_polygon_drawing_start();
            else return on_polygon_drawing_stop();
        case Qt::Key_H:
            if (!is_hole_draw_state) return on_hole_drawing_start();
            else return on_hole_drawing_stop();
        case Qt::Key_Return:
            if (!is_polygon_loaded_successfully) return false;
            point_marks = geom::algorithms::get_points_types(polygon, holes);
            split_diagonals = geom::algorithms::get_tri_split(polygon);
            return true;
            break;
        case Qt::Key_S:
            return save_state();

        case Qt::Key_L:
            return load_state();
    }
    return false;
}

bool app_viewer::save_state() {
    std::string filename = QFileDialog::getSaveFileName(
            get_wnd(),
            "Save Points"
            ).toStdString();
    if (filename == "") return false;
    std::ofstream out(filename.c_str());
    out << polygon.size() << endl;
    for (int i = 0; i < (int) polygon.size(); i++) {
        out << polygon[i] << endl;
    }
    for (polygon_type hl : holes) {
        out << hl.size() << endl;
        for (int i = 0; i < (int) hl.size(); i++) {
            out << hl[i] << endl;
        }
    }
    out.close();
    return true;
}

bool app_viewer::load_state() {
    std::string filename = QFileDialog::getOpenFileName(
            get_wnd(),
            "Load Points"
            ).toStdString();
    if (filename == "") return false;
    std::ifstream in(filename.c_str());

    int curI = 0;
    while (true) {
        int n;
        in >> n;
        if (in.eof()) break;
        point_type p;
        if (curI == 0) {
            on_polygon_drawing_start();
        } else {
            on_hole_drawing_start();
        }
        for (int i = 0; i < n; i++) {
            in >> p;
            cur_drawing_pts.push_back(p);
        }
        if (curI == 0) {
            on_polygon_drawing_stop();
        } else {
            on_hole_drawing_stop();
        }
        curI++;
    }
    return true;
}

bool app_viewer::on_polygon_drawing_click(point_type const & pt) {
    cur_drawing_pts.push_back(pt);
    return true;
}

bool app_viewer::on_polygon_drawing_start() {
    if (!is_hole_draw_state)
        restore_init_state();
    is_polygon_draw_state = true;
    error_str = "";
    return true;
}

bool app_viewer::on_polygon_drawing_stop() {

    if (is_hole_draw_state)
        geom::algorithms::orient_polygon_clockwise(cur_drawing_pts);
    else
        geom::algorithms::orient_polygon_anticlockwise(cur_drawing_pts);

    if (cur_drawing_pts.size() < 3) {
        error_str = "Less than 3 point.";
    }
    if (geom::algorithms::check_intersections(cur_drawing_pts)) {
        error_str = "Intersections detected.";
    }
    if (is_hole_draw_state) {
        if (!geom::algorithms::is_polygon_inside(polygon, cur_drawing_pts)) {
            error_str = "The hole is outside of polygon. ";
        }
        if (geom::algorithms::is_polygons_intersected(holes, cur_drawing_pts)) {
            error_str = "The hole is intersected with an other hole";
        }
    }
    if (error_str != "") {
        if (is_hole_draw_state)
            error_str += " The hole is deleted. Try again.";
        else {
            error_str += " The polygon is deleted. Try again.";
            is_polygon_loaded_successfully = false;
        }

    } else {
        is_polygon_loaded_successfully = true;
    }
    is_polygon_draw_state = false;
    if (error_str.empty()) {
        if (!is_hole_draw_state) {
            polygon = cur_drawing_pts;
        } else {
            holes.push_back(cur_drawing_pts);
        }
    }
    cur_drawing_pts.clear();
    return true;
}

bool app_viewer::on_triangulate() {
    if (!is_polygon_loaded_successfully) return false;
    tri_segms.clear();
    geom::algorithms::triangulate_monotonous(polygon, tri_segms);
    return true;
}

void app_viewer::restore_init_state() {
    error_str = "";
    is_polygon_draw_state = false;
    is_polygon_loaded_successfully = false;
    is_hole_draw_state = false;
    point_marks.clear();
    cur_drawing_pts.clear();
    polygon.clear();
    holes.clear();
    tri_segms.clear();
}

bool app_viewer::on_hole_drawing_start() {
    if (!is_polygon_loaded_successfully) {
        error_str = "Can't draw holes in empty polygon";
        is_hole_draw_state = false;
        return true;
    }
    is_hole_draw_state = true;
    return on_polygon_drawing_start();
}

bool app_viewer::on_hole_drawing_stop() {
    bool res = on_polygon_drawing_stop();
    is_hole_draw_state = false;
    return res;
}
