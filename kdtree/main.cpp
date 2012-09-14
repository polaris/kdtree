#include "KdPoint.h"
#include "KdTree.h"

#include <iostream>
#include <vector>

int main()
{
    std::vector<KdPoint<3> > pts;
    
    float a[3] = { 2, 3, 0 };
    float b[3] = { 5, 4, 0 };
    float c[3] = { 9, 6, 0 };
    float d[3] = { 4, 7, 0 };
    float g[3] = { 8, 1, 0 };
    float h[3] = { 7, 2, 0 };
    
    float e[3] = { 9, 2, 0 };
    float f[3] = { -1, -1, -1 };
    
    pts.push_back(KdPoint<3>(a));
    pts.push_back(KdPoint<3>(b));
    pts.push_back(KdPoint<3>(c));
    pts.push_back(KdPoint<3>(d));
    pts.push_back(KdPoint<3>(g));
    pts.push_back(KdPoint<3>(h));

    KdTree<3> t2(pts);

    KdPoint<3> q(e), res(f);
    float dist;
    t2.nearest(q, res, dist);
    
    std::cout << "found " << res[0] << ", " << res[1] << ", " << res[2] << std::endl;
    std::cout << "distance " << dist << std::endl;
}
