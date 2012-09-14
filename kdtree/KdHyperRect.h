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

#ifndef __KDHYPERRECT_H
#define __KDHYPERRECT_H

#include "KdPoint.h"
#include <vector>

template <int DIM>
struct KdHyperRect
{
    KdHyperRect()
    {
        memset(min, 0, DIM * sizeof(float));
        memset(max, 0, DIM * sizeof(float));
    }

    KdHyperRect(const KdHyperRect &rect)
    {
        memcpy(max, rect.max, DIM * sizeof(float));
        memcpy(min, rect.min, DIM * sizeof(float));
    }

    void extend(const std::vector<KdPoint<DIM> > &pts)
    {
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

#endif  // __KDHYPERRECT_H