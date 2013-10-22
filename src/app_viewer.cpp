#include "app_viewer.h"
#include "stdafx.h"
#include "drawer.h"
#include "algorithms.h"

void app_viewer::draw(drawer_type & drawer) const {
    drawer.set_color(Qt::blue);
    for (point_type const & pt : pts_)
        drawer.draw_point(pt, 3);

    drawer.set_color(Qt::red);
    for (uint i = 1; i < pts_.size(); i++) {
        if (is_polygon_draw_state)
            drawer.draw_line(segment_type(pts_[i - 1], pts_[i]));
        else
            drawer::drawArrow(drawer, segment_type(pts_[i - 1], pts_[i]));
    }

    if (!is_polygon_draw_state && pts_.size() > 1)
        drawer::drawArrow(drawer, segment_type(*pts_.rbegin(), *pts_.begin()));
    
    drawer.set_color(Qt::yellow);
    for(segment_type sg : tri_segms_) {
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
    printer.corner_stream() << "Points num: " << pts_.size() << endl;
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
        case Qt::Key_Return:
            if (pts_.size() >= 2) {
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
                boost::copy(pts_, std::ostream_iterator<point_type>(out, "\n"));
            }
        }
            break;
        case Qt::Key_L:
        {
            std::string filename = QFileDialog::getOpenFileName(
                    get_wnd(),
                    "Load Points"
                    ).toStdString();
            if (filename != "") {
                std::ifstream in(filename.c_str());
                std::istream_iterator<point_type> beg(in), end;

                pts_.assign(beg, end);
                cnt_.reset();
                // TODO: update it when holes will be available
                return on_polygon_drawing_stop();
            }
        }
    }
    return false;
}

bool app_viewer::on_polygon_drawing_click(point_type const & pt) {
    pts_.push_back(pt);
    cnt_.reset();
    return true;
}

bool app_viewer::on_polygon_drawing_start() {
    is_polygon_draw_state = true;
    restore_init_state();
    return true;
}

bool app_viewer::on_polygon_drawing_stop() {
    // check correctness
    error_str = "";
    geom::algorithms::orient_polygon_anticlockwise(pts_);
    if (pts_.size() < 3) {
        error_str = "Less than 3 point.";
    }
    if (geom::algorithms::check_intersections(pts_)) {
        error_str = "Intersections detected.";
    }

    if (error_str.size() != 0) {
        error_str = error_str + " Polygon is deleted. Try again. ";
        is_polygon_loaded_successfully = false;
    } else {
        is_polygon_loaded_successfully = true;
    }
    is_polygon_draw_state = false;
    return true;
}

bool app_viewer::on_triangulate() {
    tri_segms_.clear();
    geom::algorithms::triangulate_monotonous(pts_, tri_segms_);
    return true;
}

void app_viewer::restore_init_state() {
    error_str = "";
    pts_.clear();
    tri_segms_.clear();
}