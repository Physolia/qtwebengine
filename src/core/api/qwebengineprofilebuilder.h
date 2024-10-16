// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWEBENGINEPROFILEBUILDER_H
#define QWEBENGINEPROFILEBUILDER_H

#include <QtCore/QScopedPointer>
#include <QtWebEngineCore/qtwebenginecoreglobal.h>
#include <QtWebEngineCore/qwebengineprofile.h>

QT_BEGIN_NAMESPACE

struct QWebEngineProfileBuilderPrivate;
class Q_WEBENGINECORE_EXPORT QWebEngineProfileBuilder
{
public:
    QWebEngineProfileBuilder();
    ~QWebEngineProfileBuilder();
    QWebEngineProfile *createProfile(const QString &storageName, QObject *parent = nullptr);
    static QWebEngineProfile *createOffTheRecordProfile(QObject *parent = nullptr);
    QWebEngineProfileBuilder &setPersistentStoragePath(const QString &path);
    QWebEngineProfileBuilder &setCachePath(const QString &path);
    QWebEngineProfileBuilder &setHttpCacheType(QWebEngineProfile::HttpCacheType httpCacheType);
    QWebEngineProfileBuilder &setPersistentCookiesPolicy(
            QWebEngineProfile::PersistentCookiesPolicy persistentCookiesPolicy);
    QWebEngineProfileBuilder &setHttpCacheMaximumSize(int maxSizeInBytes);
    QWebEngineProfileBuilder &setPersistentPermissionsPolicy(
            QWebEngineProfile::PersistentPermissionsPolicy persistentPermissionPolicy);

private:
    Q_DISABLE_COPY(QWebEngineProfileBuilder)
    std::unique_ptr<QWebEngineProfileBuilderPrivate> d_ptr;
};

QT_END_NAMESPACE

#endif // QWEBENGINEPROFILEBUILDER_H
