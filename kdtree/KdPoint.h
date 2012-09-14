#ifndef __KDPOINT_H
#define __KDPOINT_H

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

    const float & operator [](int i) const
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
static float dist_sq(const KdPoint<DIM> &p, const KdPoint<DIM> &q)
{
    float result = 0, d = 0;
    for (int i = 0; i < DIM; i++) {
        d = q[i] - p[i];
        result += d * d;
    }
    return result;
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

#endif  // __KDPOINT_H