// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BROWSER_ACCESSIBILITY_QT_H
#define BROWSER_ACCESSIBILITY_QT_H

#include <QtGui/qtguiglobal.h>

#if QT_CONFIG(accessibility)
QT_FORWARD_DECLARE_CLASS(QAccessibleInterface)

namespace ui {
class BrowserAccessibility;

QAccessibleInterface *toQAccessibleInterface(BrowserAccessibility *obj);
const QAccessibleInterface *toQAccessibleInterface(const BrowserAccessibility *obj);

} // namespace ui
#endif // QT_CONFIG(accessibility)

#endif // BROWSER_ACCESSIBILITY_QT_H
