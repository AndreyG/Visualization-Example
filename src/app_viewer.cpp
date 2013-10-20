#include "app_viewer.h"
#include "stdafx.h"
#include "drawer.h"

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
        global_stream2 << endl << "Press SPACE button to STOP polygon drawing (last face will be added automatically)";
    } else {
        global_stream2 << endl << "Press SPACE button to START polygon drawing (remove previous).";
    }


}

bool app_viewer::on_double_click(point_type const & pt) {
    if (is_polygon_draw_state) {
        return on_polygon_drawing_click(pt);
    }
    return false;
}

bool app_viewer::on_key(int key) {
    switch (key) {
        case Qt::Key_Space:
            is_polygon_draw_state = !is_polygon_draw_state;
            if (is_polygon_draw_state) return on_polygon_drawing_start();
            else return on_polygon_drawing_stop();
        case Qt::Key_Return:
            if (pts_.size() >= 2) {
                //            cnt_.reset(new contour_type(geom::algorithms::convex_hull::andrews(pts_)));
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
                return true;
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
    restore_init_state();
    return true;
}

bool app_viewer::on_polygon_drawing_stop() {
    // check correctness
    error_str = "";
    if (pts_.size() < 3) {
        error_str = "Less than 3 point.";
    } else {
        pts_.push_back(pts_[0]);
    }
    if (error_str.size() != 0) {
        error_str = error_str + " Polygon is deleted. Try again. ";
    }
    return true;
}

void app_viewer::restore_init_state() {
    error_str = "";
    pts_.clear();
}