/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2025 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#ifndef MAPNIK_MARKERS_PLACEMENTS_POLYLABEL_HPP
#define MAPNIK_MARKERS_PLACEMENTS_POLYLABEL_HPP

#include <mapnik/markers_placements/point.hpp>
#include <mapnik/geom_util.hpp>
#include <mapnik/geometry/geometry_types.hpp>
#include <mapnik/geometry/polygon_vertex_processor.hpp>

#include <mapnik/geometry/point.hpp>
#include <mapnik/geometry/polylabel.hpp>

namespace mapnik {

template<typename Locator, typename Detector>
class markers_polylabel_placement : public markers_point_placement<Locator, Detector>
{
  public:
    using point_placement = markers_point_placement<Locator, Detector>;
    using point_placement::point_placement;

    bool get_point(double& x, double& y, double& angle, bool ignore_placement)
    {
        if (this->done_)
        {
            return false;
        }

        if (this->locator_.type() != geometry::geometry_types::Polygon)
        {
            return point_placement::get_point(x, y, angle, ignore_placement);
        }

        geometry::polygon_vertex_processor<double> vertex_processor;
        vertex_processor.add_path(this->locator_);
        double precision = geometry::polylabel_precision(vertex_processor.polygon_, this->params_.scale_factor);
        geometry::point<double> placement;
        if (!geometry::polylabel(vertex_processor.polygon_, precision, placement))
        {
            this->done_ = true;
            return false;
        }

        x = placement.x;
        y = placement.y;
        angle = 0;

        if (!this->push_to_detector(x, y, angle, ignore_placement))
        {
            return false;
        }

        this->done_ = true;
        return true;
    }
};

} // namespace mapnik

#endif // MAPNIK_MARKERS_PLACEMENTS_POLYLABEL_HPP
