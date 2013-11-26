#pragma once

#include "triangulation_types.h"
#include <vector>
#include <map>

class Status {

public:
	// Add new segment by vertex (left end)
	// next or prev will be chosen as 
	// right end (depends on type).
	// See get_right_end() for details.
	void add(const PolygonVertex* v);

	// Remove segment with right end v, it is possible
	// that segment will not be found.
	void remove(const PolygonVertex* v);

	// Find point u such that u and v upper than some segment 
	// but not lie together on it, otherwise return NULL.
	const PolygonVertex* get_helper(const PolygonVertex* v);
	
	// It's a TRIP_MERGE vertex case. This vertex can has two
	// helpers (upper and lower)
	const PolygonVertex* get_lower_helper(const PolygonVertex* v);

	Status() :
			helper(segments) {
	}

private:
	std::vector<PolygonHoleSegment> segments;
	std::map<const PolygonVertex*, size_t> rightEndToSegment;

	struct HelperComparator {
		const std::vector<PolygonHoleSegment>& segments;
		HelperComparator(const std::vector<PolygonHoleSegment>& segments_) :
				segments(segments_) {
		}
		bool operator()(const size_t& a, const size_t b);
	};

	// Maps index from segments to helper,
	// all keys in map are used to determine
	// whether segment active or not
	typedef map<size_t, const PolygonVertex*, HelperComparator> helper_type;
	helper_type helper;

	void update_helper(const PolygonVertex* v);
	const PolygonVertex* get_right_end(const PolygonVertex* v);
	
	helper_type::iterator find_bound(const PolygonVertex* v);

};
