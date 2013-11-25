#include "triangulation_types.h" 

void TriPolygon::add_vertex(const point_type& point) {
	vertexes.push_back(PolygonVertex(this, vertexes.size(), point));
}

std::ostream & operator <<(std::ostream & out, PolygonVertex const & pv) {
	out << pv.index << "(" << pv.point.x << ", " << pv.point.y << ")";
	return out;
}
