#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <stdexcept>

template <int DIM>
class KdPoint
{
public:
    KdPoint()
    {
        memset(v_, 0, DIM * sizeof(float));
    }

    KdPoint(float * const v)
    {
        memcpy(v_, v, DIM * sizeof(float));
    }

    float operator [](int i) const
    {
        if (i >= DIM) {
            throw std::runtime_error("index must be smaller than DIM");
        }
        return v_[i];
    }

private:
    float v_[DIM];
};

template <int DIM>
static float distance(const KdPoint<DIM> &p, const KdPoint<DIM> &q)
{
    float result = 0, d = 0;
    for (int i = 0; i < DIM; i++) {
        d = q[i] - p[i];
        result = d * d;
    }
    return sqrtf(d);
}

template <int DIM>
class KdPointComparer
{
public:
    KdPointComparer(int axis)
    : axis_(axis)
    {
        if (axis >= DIM) {
            throw std::runtime_error("axis must be smaller than DIM");
        }
    }

    bool operator ()(const KdPoint<DIM> &a, const KdPoint<DIM> &b) const
    {
        return a[axis_] < b[axis_];
    }

private:
    const int axis_;
};

template <int DIM>
struct KdHyperRect
{
    KdHyperRect()
    {
        memset(min, 0, DIM * sizeof(float));
        memset(max, 0, DIM * sizeof(float));
    }

    KdHyperRect(const std::vector<KdPoint<DIM> > &pts)
    {
        memset(min, 0, DIM * sizeof(float));
        memset(max, 0, DIM * sizeof(float));

        for (size_t i = 0; i < pts.size(); i++) {
            extend(pts[i]);
        }
    }

    void extend(const KdPoint<DIM> &pt)
    {
        for (int i = 0; i < DIM; ++i) {
            if (pt[i] < min[i]) {
                min[i] = pt[i];
            }
            if (pt[i] > max[i]) {
                max[i] = pt[i];
            }
        }
    }

    float dist_sq(const KdPoint<DIM> &pt) const
    {
        float result = 0;
        for (int i = 0; i < DIM; i++) {
            if (pt[i] < min[i]) {
                const float d = min[i] - pt[i];
                result += d * d;
            } else if (pt[i] > max[i]) {
                const float d = max[i] - pt[i];
                result += d * d;
            }
        }
        return result;
    }

    float min[DIM];
    float max[DIM];
};

template <int DIM>
struct KdTreeNode
{
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
            left_ = new KdTreeNode(axis_ + 1 % DIM, left);
        } else {
            left_ = new KdTreeNode(axis_ + 1 % DIM, left[0]);
        }

        if (right.size() > 1) {
            right_ = new KdTreeNode(axis_ + 1 % DIM, right);
        } else {
            right_ = new KdTreeNode(axis_ + 1 % DIM, right[0]);
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

    void nearest(const KdPoint<DIM> &pt, KdPoint<DIM> &res, float &d)
    {
        if (left_ == 0 && right_ == 0) {
            const float td = distance<DIM>(pt_, pt);
            if (td < d) {
                d = td;
                res = pt_;
            }
        } else {
            if (pt[axis_] < position_) {
                if (pt[axis_] - d < position_) {
                    left_->nearest(pt, res, d);
                }
                if (pt[axis_] + d >= position_) {
                    right_->nearest(pt, res, d);
                }
            } else {
                if (pt[axis_] + d >= position_) {
                    right_->nearest(pt, res, d);
                }
                if (pt[axis_] - d < position_) {
                    left_->nearest(pt, res, d);
                }
            }
        }
    }

    int axis_;
    KdPoint<DIM> pt_;
    float position_;
    KdTreeNode *left_, *right_;
};

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

    void nearest(const KdPoint<DIM> &pt, KdPoint<DIM> &res) const
    {
        if (root_) {
            float d = std::numeric_limits<float>::max();
            root_->nearest(pt, res, d);
        }
    }

private:
    KdTreeNode<DIM> * const root_;
    KdHyperRect<DIM> rect_;
};

int main()
{
    std::vector<KdPoint<3> > pts;
    
    float a[3] = { 0, 0, 0 };
    float b[3] = { 0, 2, 0 };
    float c[3] = { 2, 0, 0 };
    float d[3] = { 2, 2, 0 };
    float e[3] = { 2, -1, 0 };
    float f[3] = { -1, -1, -1 };
    
    pts.push_back(KdPoint<3>(a));
    pts.push_back(KdPoint<3>(b));
    pts.push_back(KdPoint<3>(c));
    pts.push_back(KdPoint<3>(d));

    KdTree<3> t2(pts);

    KdPoint<3> q(e), res(f);
    t2.nearest(q, res);
    
    std::cout << res[0] << ", " << res[1] << ", " << res[2] << std::endl;
}
