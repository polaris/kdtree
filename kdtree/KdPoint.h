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

#ifndef __KDPOINT_H
#define __KDPOINT_H

#include <stdexcept>


template <int DIM>
struct KdPoint
{
    const float & operator [](int i) const
    {
        if (i >= DIM) {
            throw std::runtime_error("index must be smaller than DIM");
        }
        return v_[i];
    }

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

    KdPointComparer &operator=(const KdPointComparer &);
};

#endif  // __KDPOINT_H