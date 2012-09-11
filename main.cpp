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

    float v_[3];
};

static float distance(const Point3d &p, const Point3d &q)
{
    const float dx = q.v_[0] - p.v_[0];
    const float dy = q.v_[1] - p.v_[1];
    const float dz = q.v_[2] - p.v_[2];
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
        return a.v_[axis_] < b.v_[axis_];
    }

private:
    int axis_;
};

template <int DIM>
struct KdTreeNode
{
    KdTreeNode(int axis, const Point3d &pt)
    : axis_(axis), pt_(pt), position_(pt.v_[axis]), left_(0), right_(0)
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

        position_ = temp[median].v_[axis_];

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
            if (pt.v_[axis_] < position_) {
                if (pt.v_[axis_] - d < position_) {
                    left_->nearest(pt, res, d);
                }
                if (pt.v_[axis_] + d >= position_) {
                    right_->nearest(pt, res, d);
                }
            } else {
                if (pt.v_[axis_] + d >= position_) {
                    right_->nearest(pt, res, d);
                }
                if (pt.v_[axis_] - d < position_) {
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
};

int main()
{
    std::vector<Point3d> pts;
    pts.push_back(Point3d(0, 0, 0));
    pts.push_back(Point3d(0, 0, 1));
    pts.push_back(Point3d(0, 1, 0));
    pts.push_back(Point3d(0, 1, 1));
    pts.push_back(Point3d(1, 0, 0));
    pts.push_back(Point3d(1, 0, 1));
    pts.push_back(Point3d(1, 1, 0));
    pts.push_back(Point3d(1, 1, 1));

    KdTree<3> t2(pts);

    Point3d q(0.39f, 0.39f, 0.39f), res(-1, -1, -1);
    t2.nearest(q, res);
}
