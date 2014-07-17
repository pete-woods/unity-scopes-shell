/*
 * Copyright (C) 2014 Canonical, Ltd.
 *
 * Authors:
 *  Michal Hruby <michal.hruby@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Self
#include "overviewscope.h"

// local
#include "overviewcategories.h"
#include "utils.h"

// Qt
#include <QScopedPointer>

namespace scopes_ng
{

using namespace unity;

OverviewScope::OverviewScope(QObject *parent) : scopes_ng::Scope(parent)
{
    m_categories.reset(new OverviewCategories(this));
}

OverviewScope::~OverviewScope()
{
}

QString OverviewScope::id() const
{
    return QString("scopesOverview");
}

void OverviewScope::setSearchQuery(const QString& search_query)
{
    OverviewCategories* categories = qobject_cast<OverviewCategories*>(m_categories.data());
    if (!categories) {
        qWarning("Unable to cast m_categories to OverviewCategories");
        return;
    }

    categories->setSurfacingMode(search_query.isEmpty());

    if (!search_query.isEmpty()) {
        Scope::setSearchQuery(search_query);
    }
}

} // namespace scopes_ng
