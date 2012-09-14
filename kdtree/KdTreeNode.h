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

#ifndef __KDTREENODE_H
#define __KDTREENODE_H

#include "KdHyperRect.h"
#include "KdPoint.h"

#include <vector>
#include <algorithm>

template <int DIM>
class KdTreeNode
{
public:
    KdTreeNode(int axis, const KdPoint<DIM> &pt)
    : axis_(axis), pt_(pt), position_(pt[axis]), left_(0), right_(0)
    {
    }

    KdTreeNode(int axis, const std::vector<KdPoint<DIM> > &pts)
    : axis_(axis), left_(0), right_(0)
    {
        std::vector<KdPoint<DIM> > temp(pts.begin(), pts.end());

        const KdPointComparer<DIM> pointComparer(axis_);
        std::sort(temp.begin(), temp.end(), pointComparer);

        const size_t median = temp.size() / 2;

        std::vector<KdPoint<DIM> > left(temp.begin(), temp.begin() + median);
        std::vector<KdPoint<DIM> > right(temp.begin() + median, temp.end());

        position_ = temp[median][axis_];

        if (left.size() > 1) {
            left_ = new KdTreeNode((axis_ + 1) % DIM, left);
        } else {
            left_ = new KdTreeNode((axis_ + 1) % DIM, left[0]);
        }

        if (right.size() > 1) {
            right_ = new KdTreeNode((axis_ + 1) % DIM, right);
        } else {
            right_ = new KdTreeNode((axis_ + 1) % DIM, right[0]);
        }
    }

    ~KdTreeNode()
    {
        if (left_) {
            delete left_;
        }

        if (right_) {
            delete right_;
        }
    }

    void nearest(const KdPoint<DIM> &pt, KdPoint<DIM> &res, float &d, const KdHyperRect<DIM> &rect) const
    {
        if (left_ == 0 && right_ == 0) {
            const float td = dist_sq<DIM>(pt_, pt);
            if (td < d) {
                d = td;
                res = pt_;
            }
        } else {
            KdTreeNode *near_subtree, *far_subtree;
            float *near_coord, *far_coord, temp;

            // Decide which half of the hyperrect to test for candidates first.
            if (pt[axis_] < position_) {
                near_subtree = left_;
                far_subtree = right_;
                near_coord = (float *)(& rect.max[axis_]);
                far_coord = (float *)(& rect.min[axis_]);
            } else {
                near_subtree = right_;
                far_subtree = left_;
                near_coord = (float *)(& rect.min[axis_]);
                far_coord = (float *)(& rect.max[axis_]);
            }

            // Modify the hyperrect to represent the nearer half.
            temp = *near_coord;
            *near_coord = position_;
            near_subtree->nearest(pt, res, d, rect);
            *near_coord = temp;

            // Modifiy the hyperrect to represent the farer half.
            temp = *far_coord;
            *far_coord = position_;
            // Decide whether it is necessary to test the farer half for candidates.
            if (rect.dist_sq(pt) < d) {
                far_subtree->nearest(pt, res, d, rect);
            }
            *far_coord = temp;
        }
    }

private:
    int axis_;
    KdPoint<DIM> pt_;
    float position_;
    KdTreeNode *left_, *right_;
};


#endif  // __KDTREENODE_H