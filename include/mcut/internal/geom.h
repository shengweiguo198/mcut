/**
 * Copyright (c) 2020-2021 CutDigital Ltd.
 * All rights reserved.
 * 
 * NOTE: This file is licensed under GPL-3.0-or-later (default). 
 * A commercial license can be purchased from CutDigital Ltd. 
 *  
 * License details:
 * 
 * (A)  GNU General Public License ("GPL"); a copy of which you should have 
 *      recieved with this file.
 * 	    - see also: <http://www.gnu.org/licenses/>
 * (B)  Commercial license.
 *      - email: contact@cut-digital.com
 * 
 * The commercial license options is for users that wish to use MCUT in 
 * their products for comercial purposes but do not wish to release their 
 * software products under the GPL license. 
 * 
 * Author(s)     : Floyd M. Chitalu
 */

#ifndef MCUT_GEOM_H_
#define MCUT_GEOM_H_

#include "mcut/internal/math.h"

namespace mcut {
namespace geom {

    // Compute a polygon's plane coefficients (i.e. normal and d parameters).
    // The computed normal is not normalized. This function returns the largest component of the normal.
    int compute_polygon_plane_coefficients(
        math::vec3& normal,
        math::real_number_t& d_coeff,
        const math::vec3* polygon_vertices,
        const int polygon_vertex_count);

    // Test if a line segment intersects with a plane, and yeild the intersection point if so.
    //
    // Return values:
    // 'p': The segment lies wholly within the plane.
    // 'q': The(first) q endpoint is on the plane (but not 'p').
    // 'r' : The(second) r endpoint is on the plane (but not 'p').
    // '0' : The segment lies strictly to one side or the other of the plane.
    // '1': The segment intersects the plane, and none of {p, q, r} hold.
    char compute_segment_plane_intersection(
        math::vec3& p,
        const math::vec3& normal,
        const math::real_number_t& d_coeff,
        const math::vec3& q,
        const math::vec3& r);

    // Test if a point 'q' (in 2D) lies inside or outside a given polygon (count the number ray crossings).
    //
    // Return values:
    // 'i': q is strictly interior
    // 'o': q is strictly exterior (outside).
    // 'e': q is on an edge, but not an endpoint.
    // 'v': q is a vertex.
    char compute_point_in_polygon_test(
        const math::vec2& q,
        const math::vec2* polygon_vertices,
        const int polygon_vertex_count);

    // Test if a point 'q' (in 3D) lies inside or outside a given polygon (count the number ray crossings).
    //
    // Return values:
    // 'i': q is strictly interior
    // 'o': q is strictly exterior (outside).
    // 'e': q is on an edge, but not an endpoint.
    // 'v': q is a vertex.
    char compute_point_in_polygon_test(
        const math::vec3& p,
        const math::vec3* polygon_vertices,
        const int polygon_vertex_count,
        const int polygon_plane_normal_largest_component);

    template <typename vector_type>
    struct bounding_box_t {

        vector_type m_minimum;
        vector_type m_maximum;

        bounding_box_t(const vector_type& minimum, const vector_type& maximum)
        {
            m_minimum = minimum;
            m_maximum = maximum;
        }

        bounding_box_t()
        {
            m_minimum = vector_type(std::numeric_limits<double>::max());
            m_maximum = vector_type(std::numeric_limits<double>::min());
        }

        const vector_type& minimum() const
        {
            return m_minimum;
        }

        const vector_type& maximum() const
        {
            return m_maximum;
        }

        void expand(const vector_type& point)
        {
            m_maximum = compwise_max(m_maximum, point);
            m_minimum = compwise_min(m_minimum, point);
        }

        void expand(const bounding_box_t<vector_type>& bbox)
        {
            m_maximum = compwise_max(m_maximum, bbox.maximum());
            m_minimum = compwise_min(m_minimum, bbox.minimum());
        }
    };

    template <typename T>
    bool intersect_bounding_boxes(const bounding_box_t<math::vec3_<T>>& a, const bounding_box_t<math::vec3_<T>>& b)
    {
        return (a.minimum().x() <= b.maximum().x() && a.maximum().x() >= b.minimum().x()) && //
            (a.minimum().y() <= b.maximum().y() && a.maximum().y() >= b.minimum().y()) && //
            (a.minimum().z() <= b.maximum().z() && a.maximum().z() >= b.minimum().z());
    }

    bool point_in_bounding_box(const math::vec2& point, const bounding_box_t<math::vec2>& bbox);

    bool point_in_bounding_box(const math::vec3& point, const bounding_box_t<math::vec3>& bbox);

    template <typename vector_type>
    void make_bbox(bounding_box_t<vector_type>& bbox, const vector_type* vertices, const int num_vertices)
    {
        MCUT_ASSERT(vertices != nullptr);
        MCUT_ASSERT(num_vertices >= 3);

        for (int i = 0; i < num_vertices; ++i) {
            const vector_type& vertex = vertices[i];
            bbox.expand(vertex);
        }
    }
} // namespace geom {
} // namespace mcut {

#endif // MCUT_GEOM_H_
