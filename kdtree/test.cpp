/**
 * Copyright (c) 2012 Jan Deinhard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include "KdPoint.h"
#include "KdTree.h"

#include <iostream>
#include <vector>

int main()
{
    KdPoint<3> pts[] = {{ 2, 3, 0 }, 
                        { 5, 4, 0 },
                        { 9, 6, 0 },
                        { 4, 7, 0 },
                        { 8, 1, 0 },
                        { 7, 2, 0 }};

    KdTree<3> t2(pts, 6);

    KdPoint<3> q = { 9, 2, 0 }, res;
    float dist;
    t2.nearest(q, res, dist);
    
    std::cout << "found " << res[0] << ", " << res[1] << ", " << res[2] << std::endl;
    std::cout << "distance " << dist << std::endl;
}
