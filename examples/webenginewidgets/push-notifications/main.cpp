// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "notificationpopup.h"

#include <QApplication>
#include <QWebEngineProfile>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QWebEngineProfileBuilder>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("QtExamples");
    QApplication app(argc, argv);

    const QString name =
            QString::fromLatin1("push-notifications.%1").arg(qWebEngineChromiumVersion());

    QWebEngineProfileBuilder profileBuilder;
    QScopedPointer<QWebEngineProfile> profile(profileBuilder.createProfile(name));
    QWebEngineView view(profile.data());
    auto popup = new NotificationPopup(&view);

    QObject::connect(view.page(), &QWebEnginePage::permissionRequested,
                     [&](QWebEnginePermission permission) {
                         if (permission.permissionType() != QWebEnginePermission::PermissionType::Notifications)
                             return;

                         permission.grant();
                     });

    profile->setPushServiceEnabled(true);
    profile->setNotificationPresenter([&](std::unique_ptr<QWebEngineNotification> notification) {
        popup->present(notification);
    });

    view.resize(640, 480);
    view.setUrl(QUrl("http://localhost:5000"));
    view.show();
    return app.exec();
}
