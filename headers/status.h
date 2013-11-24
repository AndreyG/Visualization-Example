#pragma once

#include "geom/primitives/point.h"
#include "geom/primitives/segment.h"
#include "triangulation_types.h"
#include "algorithms_geom.h"
#include <algorithm>

#include <vector>
#include <iostream>

using namespace std;
using namespace geom::structures;
using namespace geom::algorithms;

class Status {
	segments;

public:

	void add(const PolygonVertex& v) {
		TRIP_TYPE type = v.type;
		if (type == TRIP_START || type == TRIP_SPLIT) {
			insert(v.next());
		}
		if (type == TRIP_REGULAR && v.is_lower_regular()) {
			insert(v.next());
		}
	}

	void update(const PolygonVertex& v) {
		TRIP_TYPE type = v.type;
		if (type == TRIP_REGULAR && v.is_lower_regular())
			return;
		if (type == TRIP_START)
			return;
		if (type == TRIP_END)
			return;

		auto left = find_lower_segment(v);
		if (left == (size_t) (-1))
			return;
		cout << "helper[" << left << "] " << helper_[left] << "->" << v.index << "\n";
		helper_[left] = v;
	}

	size_t find_lower_segment(const PolygonVertex& v) {
		cout << "search: " << v.index << ": ";
		auto lastGood = segments.end();
		for (auto it = segments.begin(); it != segments.end(); it++) {
			cout << v.prev() << " " << *it << " ";
			point_type pa = it->prev().point;
			point_type pb = it->point;
			int turn = left_turn(segment_type(pa, pb), v.point);
			cout << turn << "::";
			if (turn > 0)
				lastGood = it;
			else
				break;
		}
		cout << " -- res: " << *lastGood << endl;

		if (lastGood == segments.end()) {
			cout << "NOT FOUND LOWER SEGMENT FOR " << v.index << "!";
			cout << endl;
			return (size_t) -1;
		}

		return *lastGood;
	}

	void remove(const PolygonVertex& v) {
		cout << "remove " << v.index << endl;

		TRIP_TYPE type = v.type;
		if (type == TRIP_SPLIT || type == TRIP_START)
			return;
		if (type == TRIP_REGULAR && !v.is_lower_regular())
			return;

		auto it = find(segments.begin(), segments.end(), v);
		if (it == segments.end()) {
			cout << "NOT FOUND SEGMENT END " << v.index << " !";
			cout << endl;
			return;
		}
		segments.erase(it);
	}

	PolygonVertex* helper(const PolygonVertex& v) {
		auto it = helper_.find(v);
		if (it != helper_.end()) {
			return &*it;
		}
		return NULL;
	}

private:

	map<PolygonVertex, PolygonVertex> helper_;
	vector<PolygonVertex> segments;

	void insert(const PolygonVertex& v) {
		cout << "insert " << v.index;
		point_type leftEnd = v.prev().point;
		auto it = segments.begin();
		while (it != segments.end()) {
			auto pa = it->prev().point();
			auto pb = it->point();
			int turn = left_turn(segment_type(pa, pb), leftEnd);
			if (turn < 0)
				break;
			it++;
		}
		segments.insert(it, v);
		cout << " Now size " << segments.size() << endl;
		helper_[v] = v.prev();
	}

};
