#pragma once

#include <algorithm>
#include "geom/primitives/point.h"
#include "geom/primitives/segment.h"

#include <vector>
#include <iostream>
using namespace std;

using namespace geom::structures;

typedef std::vector<point_type> polygon_type;

enum TRIP_TYPE {
	TRIP_SPLIT, TRIP_MERGE, TRIP_START, TRIP_END, TRIP_REGULAR
};

struct PolygonVertex;
struct TriPolygon {
	std::vector<PolygonVertex> vertexes;
	bool isHole = false;

	TriPolygon(const polygon_type& pt, bool isHole_) :
			isHole(isHole_) {
		for (auto p : pt) {
			add_vertex(p);
		}
	}

	void add_vertex(const point_type& point);
	inline size_t size() const {
		return vertexes.size();
	}
	const PolygonVertex& operator[](size_t i) const {
		return vertexes.at(i);
	}
	PolygonVertex& operator[](size_t i) {
		return vertexes.at(i);
	}
};

class PolygonVertex {
	
private:
	TriPolygon* polygon;
	
public:
	size_t index;
	point_type point;
	TRIP_TYPE type;

	PolygonVertex(TriPolygon* polygon, const size_t index,
			const point_type& point) :
			polygon(polygon), index(index), point(point), type(TRIP_REGULAR) {
	}

	PolygonVertex(const PolygonVertex& other) :
			polygon(other.polygon), index(other.index), point(other.point), type(
					other.type) {
	}

	const PolygonVertex& operator=(const PolygonVertex& other) {
		this->polygon = other.polygon;
		this->index = other.index;
		this->point = other.point;
		this->type = other.type;
		return *this;
	}

	inline const PolygonVertex& next() const {
		return (*polygon)[(index + 1) % polygon->size()];
	}

	inline const PolygonVertex& prev() const {
		return (*polygon)[(index + polygon->size() - 1) % polygon->size()];
	}

	bool is_lower_regular() const {
		if (type != TRIP_REGULAR)		
			return false;
		return prev().point.x < next().point.x;
	}
	
	TriPolygon& get_polygon(){
		return *this->polygon;
	}
	
	inline bool operator ==(PolygonVertex const & b) const {
		return (polygon == b.polygon) && (index == b.index);
	}
	
};

struct PolygonHoleSegment {
	PolygonHoleSegment(const PolygonVertex& a_, const PolygonVertex& b_) :
			a(a_), b(b_) {
	}
	PolygonHoleSegment(const PolygonHoleSegment& other) :
			a(other.a), b(other.b) {
	}
	const PolygonVertex& a;
	const PolygonVertex& b;
	geom::structures::segment_type get_segment() const {
		return geom::structures::segment_type(a.point, b.point);
	}

};

inline bool operator <(PolygonVertex const & a, PolygonVertex const & b) {
	return a.point < b.point;
}

inline bool operator >(PolygonVertex const & a, PolygonVertex const & b) {
	return b < a;
}

inline bool operator !=(PolygonVertex const & a, PolygonVertex const & b) {
	return !(a == b);
}


