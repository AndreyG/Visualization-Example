#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

#define int32 int

    struct point_type
    {
        int32 x, y;

        point_type(int32 x, int32 y)
            : x(x)
            , y(y)
        {}

        point_type()
            : x(0)
            , y(0)
        {}

    };

    inline bool operator < (point_type const & a, point_type const & b)
    {
        if (a.x == b.x)
            return a.y < b.y;
        else
            return a.x < b.x;
    }

    inline bool operator > (point_type const & a, point_type const & b)
    {
        return b < a;
    }

    inline bool operator == (point_type const & a, point_type const & b)
    {
        return (a.x == b.x) && (a.y == b.y);
    }

    inline bool operator != (point_type const & a, point_type const & b)
    {
        return !(a == b);
    }

    



int main(){
	
	vector<point_type> pts;

	pts.push_back(point_type(99, 170));
	pts.push_back(point_type(-172, 158));
	pts.push_back(point_type(-223, 110));
	pts.push_back(point_type(-236, 4));
	pts.push_back(point_type(53, -84));
	pts.push_back(point_type(145, 52));
		
	vector<point_type>::iterator minIt = std::min_element(pts.begin(), pts.end());
	
	
	
	cout << "min point: " << minIt->x << " " << minIt->y << endl;
	
	
}