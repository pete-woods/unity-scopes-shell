/*
 * Copyright (C) 2011 Canonical, Ltd.
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
#include "scopes.h"

// Local
#include "scope.h"

// Qt
#include <QDebug>
#include <QTimer>

#include <unity/scopes/Registry.h>
#include <unity/scopes/Scope.h>
#include <unity/scopes/ScopeProxyFwd.h>
#include <unity/UnityExceptions.h>

namespace scopes_ng
{

using namespace unity;

void ScopeListWorker::run()
{
    try
    {
        // m_runtimeConfig should be null in most cases, and empty string is for system-wide fallback
        m_scopesRuntime = scopes::Runtime::create(m_runtimeConfig.toStdString());
        auto registry = m_scopesRuntime->registry();
        m_metadataMap = registry->list();
    }
    catch (unity::Exception const& err)
    {
        qWarning("ERROR! Caught %s", err.to_string().c_str());
    }
    Q_EMIT discoveryFinished();
}

void ScopeListWorker::setRuntimeConfig(QString const& config)
{
    m_runtimeConfig = config;
}

scopes::Runtime::UPtr ScopeListWorker::takeRuntime()
{
    return std::move(m_scopesRuntime);
}

scopes::MetadataMap ScopeListWorker::metadataMap() const
{
    return m_metadataMap;
}

int Scopes::LIST_DELAY = -1;

Scopes::Scopes(QObject *parent)
    : QAbstractListModel(parent)
    , m_listThread(nullptr)
    , m_loaded(false)
{
    m_roles[Scopes::RoleScope] = "scope";
    m_roles[Scopes::RoleId] = "id";
    m_roles[Scopes::RoleVisible] = "visible";
    m_roles[Scopes::RoleTitle] = "title";

    // delaying spawning the worker thread, causes problems with qmlplugindump
    // without it
    if (LIST_DELAY < 0) {
        QByteArray listDelay = qgetenv("UNITY_SCOPES_LIST_DELAY");
        LIST_DELAY = listDelay.isNull() ? 100 : listDelay.toInt();
    }
    QTimer::singleShot(LIST_DELAY, this, SLOT(populateScopes()));
}

Scopes::~Scopes()
{
    if (m_listThread && !m_listThread->isFinished()) {
        // FIXME: wait indefinitely once libunity-scopes supports timeouts
        m_listThread->wait(5000);
    }
}

QHash<int, QByteArray> Scopes::roleNames() const
{
    return m_roles;
}

int Scopes::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_scopes.count();
}

void Scopes::populateScopes()
{
    auto thread = new ScopeListWorker;
    QByteArray runtimeConfig = qgetenv("UNITY_SCOPES_RUNTIME_PATH");
    thread->setRuntimeConfig(QString::fromLocal8Bit(runtimeConfig));
    QObject::connect(thread, &ScopeListWorker::discoveryFinished, this, &Scopes::discoveryFinished);
    QObject::connect(thread, &ScopeListWorker::finished, thread, &QObject::deleteLater);

    m_listThread = thread;
    m_listThread->start();
}

void Scopes::discoveryFinished()
{
    ScopeListWorker* thread = qobject_cast<ScopeListWorker*>(sender());

    m_scopesRuntime = thread->takeRuntime();
    auto scopes = thread->metadataMap();

    // FIXME: use a dconf setting for this
    QByteArray enabledScopes = qgetenv("UNITY_SCOPES_LIST");

    beginResetModel();

    if (!enabledScopes.isNull()) {
        QList<QByteArray> scopeList = enabledScopes.split(';');
        for (int i = 0; i < scopeList.size(); i++) {
            std::string scope_name(scopeList[i].constData());
            auto it = scopes.find(scope_name);
            if (it != scopes.end()) {
                auto scope = new Scope(this);
                scope->setScopeData(it->second);
                m_scopes.append(scope);
            }
        }
    } else {
        // add all the scopes
        for (auto it = scopes.begin(); it != scopes.end(); ++it) {
            auto scope = new Scope(this);
            scope->setScopeData(it->second);
            m_scopes.append(scope);
        }
    }

    endResetModel();

    m_loaded = true;
    Q_EMIT loadedChanged(m_loaded);

    m_listThread = nullptr;
}

QVariant Scopes::data(const QModelIndex& index, int role) const
{
    Scope* scope = m_scopes.at(index.row());

    switch (role) {
        case Scopes::RoleScope:
            return QVariant::fromValue(scope);
        case Scopes::RoleId:
            return QString(scope->id());
        case Scopes::RoleVisible:
            return QVariant::fromValue(scope->visible());
        case Scopes::RoleTitle:
            return QString(scope->name());
        default:
            return QVariant();
    }
}

QVariant Scopes::get(int row) const
{
    if (row >= m_scopes.size() || row < 0) {
        return QVariant();
    }
    return data(QAbstractListModel::index(row), RoleScope);
}

QVariant Scopes::get(const QString& scope_id) const
{
    Q_FOREACH(Scope* scope, m_scopes) {
        if (scope->id() == scope_id) {
            return QVariant::fromValue(scope);
        }
    }

    return QVariant();
}

bool Scopes::loaded() const
{
    return m_loaded;
}

} // namespace scopes_ng