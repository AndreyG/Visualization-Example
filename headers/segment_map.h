#pragma once

#include "triangulation_types.h"
#include "algorithms_geom.h"

using namespace geom::algorithms;

class SegmentMap {

public:
	struct Node {
		friend SegmentMap;
	private:
		Node* left;
		Node* right;
		const PolygonVertex* data;
		const PolygonHoleSegment segment;
	public:

		Node(const PolygonHoleSegment& segment_, const PolygonVertex* data_) :
				data(data_), segment(segment_), left(NULL), right(NULL) {
		}
		~Node() {
			if (left != NULL)
				delete left;
			if (right != NULL)
				delete right;
		}
	};

	SegmentMap() :
			root(NULL) {
	}

	Node* root;
	void add(const PolygonHoleSegment& segment, const PolygonVertex* data) {
		tree_add(segment, data, &root);
	}

private:
	void tree_add(const PolygonHoleSegment& segment, const PolygonVertex* data,
			Node** nodep) {
		point_type leftEnd = segment.get_segment()[0];

		while (true) {
			if (*nodep == NULL) {
				*nodep = new Node(segment, data);
				break;
			}
			int turn = left_turn((**nodep).segment.get_segment(), leftEnd);
			if (turn <= 0) {
				nodep = &(**nodep).left;
			} else {
				nodep = &(**nodep).right;
			}
		}
	}
};

//
//
//
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
//

