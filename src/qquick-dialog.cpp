/*
 * This file is part of signon-ui
 *
 * Copyright (C) 2011 Canonical Ltd.
 *
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qquick-dialog.h"

#include "debug.h"

#include <QEvent>

using namespace SignOnUi::QQuick;

Dialog::Dialog(QWindow *parent):
    QQuickView(parent)
{
    setResizeMode(QQuickView::SizeRootObjectToView);
}

Dialog::~Dialog()
{
}

void Dialog::accept()
{
    done(Dialog::Accepted);
}

void Dialog::reject()
{
    done(Dialog::Rejected);
}

void Dialog::done(int result)
{
    setVisible(false);
    Q_EMIT finished(result);
}

bool Dialog::event(QEvent *e)
{
    if (e->type() == QEvent::Close) {
        reject();
    }
    return QQuickView::event(e);
}
