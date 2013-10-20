#include "visualization/viewer_adapter.h"
#include "visualization/draw_util.h"
#include "io/point.h"
#include "algorithms.h"

#include <iostream>

using namespace visualization;
using geom::structures::point_type;
using geom::structures::contour_type;

struct app_viewer : viewer_adapter
{
    void draw(drawer_type & drawer)     const;
    void print(printer_type & printer)  const;

    bool on_double_click(point_type const & pt);
    bool on_key(int key);
    
    bool on_polygon_drawing_start();
    bool on_polygon_drawing_stop();
    bool on_polygon_drawing_click(point_type const & pt);
    
    point_type get_info_pnt() const;
    point_type get_error_pnt() const;
    
    void restore_init_state();
    
private:
    std::vector<point_type>         pts_; 
    std::unique_ptr<contour_type>   cnt_;
    std::string error_str = "";
    bool is_polygon_draw_state = false;
};

