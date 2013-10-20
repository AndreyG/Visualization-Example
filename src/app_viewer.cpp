#include "app_viewer.h"
#include "stdafx.h"

void app_viewer::draw(drawer_type & drawer) const {
    drawer.set_color(Qt::blue);
    for (point_type const & pt : pts_)
        drawer.draw_point(pt, 3);

    if (cnt_) {
        drawer.set_color(Qt::red);
        visualization::draw(drawer, *cnt_);
    }
}

void app_viewer::print(printer_type & printer) const {
    printer.corner_stream() << "Points num: " << pts_.size() << endl;
    point_type bottom_left_pnt(-get_wnd()->width() / 2 + 10, 
                               -get_wnd()->height() / 2 + 10);
    stream_type& bottom_left_corner_stream = printer.global_stream(bottom_left_pnt);
    if (cnt_)
        printer.corner_stream() << "Convex hull vertices num: " << cnt_->vertices_num();
    if (is_polygon_draw_state) {
        bottom_left_corner_stream << "Press space button to STOP polygon drawing (last face will be added automatically)";
    } else {
        bottom_left_corner_stream << "Press space button to START polygon drawing";
    }
}

bool app_viewer::on_double_click(point_type const & pt) {
    //    if(is_polygon_draw_state) { 
    //        on_polygon_drawing_click(pt);
    //        return true;
    //    }
    pts_.push_back(pt);
    cnt_.reset();
    return true;
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

}

bool app_viewer::on_polygon_drawing_start() {

}

bool app_viewer::on_polygon_drawing_stop() {

}