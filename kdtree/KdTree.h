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

#ifndef __KDTREE_H
#define __KDTREE_H

#include "KdTreeNode.h"
#include "KdHyperRect.h"
#include "KdPoint.h"

#include <vector>

template <int DIM>
class KdTree
{
public:
    KdTree(const KdPoint<DIM> * const pts, int count)
    {
        const std::vector<KdPoint<DIM> > vpts(pts, pts + count);
        root_ = new KdTreeNode<DIM>(0, vpts);
        rect_.extend(vpts);
    }

    ~KdTree()
    {
        delete root_;
    }

    void nearest(const KdPoint<DIM> &pt, KdPoint<DIM> &res, float &dist) const
    {
        if (root_) {
            dist = std::numeric_limits<float>::max();
            root_->nearest(pt, res, dist, rect_);
            dist = sqrt(dist);
        }
    }

private:
    KdTreeNode<DIM> * root_;
    KdHyperRect<DIM> rect_;

    KdTree &operator =(const KdTree &);
};

#endif  // __KDTREE_H