#include "triangulation_types.h" 

void TriPolygon::add_vertex(const point_type& point) {
	verteces.push_back(PolygonVertex(*this, verteces.size(), point));
}

std::ostream & operator <<(std::ostream & out, PolygonVertex const & pv) {
	out << pv.index << "(" << pv.point.x << ", " << pv.point.y << ")";
	return out;
}
