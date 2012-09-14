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

#endif  // __KDHYPERRECT_H