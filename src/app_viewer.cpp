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
    for (uint i = 0; i < polygon.size(); i++) {
        drawer::drawArrow(drawer, segment_type(
                cur_drawing_pts[i],
                cur_drawing_pts[(i + 1) % polygon.size()])
                );
    }

    drawer.set_color(Qt::yellow);
    for (segment_type sg : tri_segms) {
        drawer.draw_line(sg);
    }

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
            -get_wnd()->height() / 2 + 30
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
}

bool app_viewer::on_double_click(point_type const & pt) {
    if (is_polygon_draw_state) {
        return on_polygon_drawing_click(pt);
    }
    return false;
}

bool app_viewer::on_key(int key) {
    cout << key << endl;
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
            if (cur_drawing_pts.size() >= 2) {
                on_triangulate();
                return true;
            }
            break;
        case Qt::Key_S:
        {
            std::string filename = QFileDialog::getSaveFileName(
                    get_wnd(),
                    "Save Points"
                    ).toStdString();
            if (filename != "") {
                std::ofstream out(filename.c_str());
                boost::copy(cur_drawing_pts, std::ostream_iterator<point_type>(out, "\n"));
            }
        }
            break;
        case Qt::Key_L:
        {
            on_polygon_drawing_start(); // imitate drawing (clear state)
            std::string filename = QFileDialog::getOpenFileName(
                    get_wnd(),
                    "Load Points"
                    ).toStdString();
            if (filename != "") {
                std::ifstream in(filename.c_str());
                std::istream_iterator<point_type> beg(in), end;
                // TODO: update it when holes will be available
                cur_drawing_pts.assign(beg, end);
                return on_polygon_drawing_stop();
            }
        }
    }
    return false;
}

bool app_viewer::on_polygon_drawing_click(point_type const & pt) {
    cur_drawing_pts.push_back(pt);
    return true;
}

bool app_viewer::on_polygon_drawing_start() {
    if (!is_hole_draw_state)
        restore_init_state();
    is_polygon_draw_state = true;
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
    if (error_str.size() != 0) {
        if (is_hole_draw_state)
            error_str += " The hole is deleted. Try again.";
        else
            error_str += " The polygon is deleted. Try again.";
        is_polygon_loaded_successfully = false;
    } else {
        is_polygon_loaded_successfully = true;
    }
    is_polygon_draw_state = false;

    if (!is_hole_draw_state) {
        polygon = cur_drawing_pts;
    } else {
        holes.push_back(cur_drawing_pts);
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
    cur_drawing_pts.clear();
    polygon.clear();
    tri_segms.clear();
    cout << "restored" << endl;
}

bool app_viewer::on_hole_drawing_start() {
    is_hole_draw_state = true;
    return on_polygon_drawing_start();
}

bool app_viewer::on_hole_drawing_stop() {
    bool res = on_polygon_drawing_stop();
    is_hole_draw_state = false;
}
