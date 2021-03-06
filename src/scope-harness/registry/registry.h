/*
 * Copyright (C) 2014 Canonical, Ltd.
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
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#pragma once

#include <unity/util/DefinesPtrs.h>

#include <QtGlobal>

namespace unity
{
namespace scopeharness
{
namespace registry
{

class Q_DECL_EXPORT Registry
{
public:
    UNITY_DEFINES_PTRS(Registry);

    virtual ~Registry() = default;

    virtual void start() = 0;

    Registry(const Registry& other) = delete;

    Registry(Registry&& other) = delete;

    Registry& operator=(const Registry& other) = delete;

    Registry& operator=(Registry&& other) = delete;

protected:
    Registry() = default;
};

}
}
}
