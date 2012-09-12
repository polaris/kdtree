#include <iostream>
#include <vector>
#include <algorithm>

struct Point3d
{
    Point3d()
    {
        v_[0] = 0;
        v_[1] = 0;
        v_[2] = 0;
    }

    Point3d(float x, float y, float z)
    {
        v_[0] = x;
        v_[1] = y;
        v_[2] = z;
    }

    float operator [](int i) const
    {
        // TODO: add bounds check
        return v_[i];
    }

    float v_[3];
};

static float distance(const Point3d &p, const Point3d &q)
{
    const float dx = q[0] - p[0];
    const float dy = q[1] - p[1];
    const float dz = q[2] - p[2];
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

class Sorter
{
public:
    Sorter(int axis)
    : axis_(axis)
    {
    }

    bool operator ()(const Point3d &a, const Point3d &b)
    {
        return a[axis_] < b[axis_];
    }

private:
    int axis_;
};

template <int DIM>
struct KdHyperRect
{
    KdHyperRect()
    {
        memset(min, 0, DIM * sizeof(float));
        memset(max, 0, DIM * sizeof(float));
    }

    KdHyperRect(const std::vector<Point3d> &pts)
    {
        memset(min, 0, DIM * sizeof(float));
        memset(max, 0, DIM * sizeof(float));

        for (size_t i = 0; i < pts.size(); i++) {
            extend(pts[i]);
        }
    }

    void extend(const Point3d &pt)
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

    float dist_sq(const Point3d &pt) const
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
    KdTreeNode(int axis, const Point3d &pt)
    : axis_(axis), pt_(pt), position_(pt[axis]), left_(0), right_(0)
    {
    }

    KdTreeNode(int axis, const std::vector<Point3d> &pts)
    : axis_(axis), left_(0), right_(0)
    {
        std::vector<Point3d> temp(pts.begin(), pts.end());

        Sorter sorter(axis_);
        std::sort(temp.begin(), temp.end(), sorter);

        const size_t median = temp.size() / 2;

        std::vector<Point3d> left(temp.begin(), temp.begin() + median);
        std::vector<Point3d> right(temp.begin() + median, temp.end());

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

    void nearest(const Point3d &pt, Point3d &res, float &d)
    {
        if (left_ == 0 && right_ == 0) {
            const float td = distance(pt_, pt);
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
    Point3d pt_;
    float position_;
    KdTreeNode *left_, *right_;
};

template <int DIM>
class KdTree
{
public:
    KdTree(const std::vector<Point3d> &pts)
    : rect_(pts)
    {
        root_ = new KdTreeNode<DIM>(0, pts);
    }

    ~KdTree()
    {
        delete root_;
    }

    void nearest(const Point3d &pt, Point3d &res) const
    {
        if (root_) {
            float d = FLT_MAX;
            root_->nearest(pt, res, d);
        }
    }

private:
    KdTreeNode<DIM> *root_;
    KdHyperRect<DIM> rect_;
};

int main()
{
    std::vector<Point3d> pts;
    pts.push_back(Point3d(0, 0, 0));
    pts.push_back(Point3d(0, 2, 0));
    pts.push_back(Point3d(2, 0, 0));
    pts.push_back(Point3d(2, 2, 0));

    KdTree<3> t2(pts);

    Point3d q(2, -1, 0), res(-1, -1, -1);
    t2.nearest(q, res);
}
