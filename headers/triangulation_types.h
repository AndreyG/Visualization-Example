#pragma once

#include <algorithm>
#include "geom/primitives/point.h"
#include "geom/primitives/segment.h"

#include <vector>

using namespace geom::structures;

enum TRIP_TYPE {
	TRIP_SPLIT, TRIP_MERGE, TRIP_START, TRIP_END, TRIP_REGULAR
};

struct PolygonVertex;
struct TriPolygon {
	std::vector<PolygonVertex> verteces;
	bool isHole = false;
	void add_vertex(const point_type& point);
	inline size_t size() const {
		return verteces.size();
	}
	const PolygonVertex& operator[](size_t i) const {
		return verteces.at(i);
	}
	PolygonVertex& operator[](size_t i) {
		return verteces.at(i);
	}
};
struct PolygonVertex {

	PolygonVertex(const TriPolygon& polygon, size_t index, point_type point) :
			polygon(polygon), index(index), point(point), type(TRIP_REGULAR) {
	}

	const size_t index;
	TRIP_TYPE type;
	point_type point;

	const TriPolygon& polygon;

	inline const PolygonVertex& next() const {
		return polygon[(index + 1) % polygon.size()];
	}

	inline const PolygonVertex& prev() const {
		return polygon[(index + polygon.size() - 1) % polygon.size()];
	}

	bool is_lower_regular() const {
		if (type != TRIP_REGULAR)
			return false;
		return next().point.x > point.x;
	}

};

struct PolygonHoleSegment {
	PolygonVertex a;
	PolygonVertex b;
	geom::structures::segment_type get_segment() {
		return geom::structures::segment_type(a.point, b.point);
	}
};
