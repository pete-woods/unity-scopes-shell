/*
 * Copyright (C) 2015 Canonical, Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pawel Stolowski <pawel.stolowski@canonical.com>
 */

#include <boost/python.hpp>
#include <scope-harness/view/abstract-view.h>

using namespace boost::python;
namespace shv = unity::scopeharness::view;

static bool compareViews(shv::AbstractView *self, shv::AbstractView *other)
{
    return self == other; // pointer comparision
}

void export_abstract_view()
{
    boost::python::register_ptr_to_python<std::shared_ptr<shv::AbstractView>>();

    class_<shv::AbstractView, boost::noncopyable>("AbstractView", no_init)
        .def("__eq__", compareViews)
        ;
}
