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


#ifndef NG_DEPARTMENT_NODE_H
#define NG_DEPARTMENT_NODE_H

#include <QSharedPointer>
#include <QMultiMap>
#include <QStringList>
#include <QPointer>

#include <unity-scopes.h>

namespace scopes_ng
{

class Q_DECL_EXPORT DepartmentNode
{
public:
    DepartmentNode(DepartmentNode* parent = nullptr);
    ~DepartmentNode();

    void initializeForDepartment(unity::scopes::Department::SCPtr const& dep);
    void initializeForFilter(unity::scopes::OptionSelectorFilter::SCPtr const& filter);
    DepartmentNode* findNodeById(QString const& id);

    QString id() const;
    QString label() const;
    QString allLabel() const;
    bool hasSubdepartments() const;
    bool isRoot() const;
    bool hidden() const;
    bool isFilter() const;

    void setIsRoot(bool isRoot);

    void appendChild(DepartmentNode* child);
    int childCount() const;
    bool isLeaf() const;
    QList<DepartmentNode*> childNodes() const;
    DepartmentNode* parent() const;
    QString filterId() const;

private:
    void clearChildren();
    void initializeForFilterOption(unity::scopes::FilterOption::SCPtr const&, QString const&);

    DepartmentNode* m_parent;
    QList<DepartmentNode*> m_children;
    QString m_id;
    QString m_label;
    QString m_allLabel;
    QString m_filterId;
    bool m_hasSubdepartments;
    bool m_isRoot;
    bool m_hidden;
    bool m_isFilter;
};

} // namespace scopes_ng

#endif // NG_DEPARTMENT_NODE_H
