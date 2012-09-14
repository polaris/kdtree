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
    KdTree(const std::vector<KdPoint<DIM> > &pts)
    : root_(new KdTreeNode<DIM>(0, pts)), rect_(pts)
    {
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
    KdTreeNode<DIM> * const root_;
    KdHyperRect<DIM> rect_;
};

#endif  // __KDTREE_H