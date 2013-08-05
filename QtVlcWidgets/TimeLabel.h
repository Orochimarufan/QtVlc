/*****************************************************************************
 * QtVlc - C++ bindings for libVLC using Qt awesomeness
 * Copyright (C) 2013 Orochimarufan <orochimarufan.x3@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
/* ATTENTION: this is GPL, not LGPL due to code from the VLC Qt Interface.
 * Original License: */
/*****************************************************************************
 * interface_widgets.hpp : Custom widgets for the main interface
 ****************************************************************************
 * Copyright (C) 2006-2008 the VideoLAN team
 * $Id: d4be935f82d11b2016afa897470dec1fe90013a6 $
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *          Rafaël Carré <funman@videolanorg>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef TIMELABEL_H
#define TIMELABEL_H

#include <QtGui/QMouseEvent>
#include <QtWidgets/QLabel>

#include "QtVlcWidgetsConfig.h"
#include "QtVlcWidgetsConst.h"

class QtVlcWidgets_EXPORT ClickableQLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableQLabel(QWidget *parent = 0) : QLabel(parent)
    {}

    virtual void mouseDoubleClickEvent(QMouseEvent *e)
    {
        Q_UNUSED(e);
        emit doubleClicked();
    }

signals:
    void doubleClicked();
};

class QtVlcWidgets_EXPORT TimeLabel : public ClickableQLabel
{
    Q_OBJECT
public:
    enum Display
    {
        Elapsed,
        Remaining,
        Both
    };

    TimeLabel(QWidget *parent = 0, TimeLabel::Display _displayType = TimeLabel::Both);

protected:
    virtual void mousePressEvent(QMouseEvent *event)
    {
        if(displayType == TimeLabel::Elapsed) return;
        toggleTimeDisplay();
        event->accept();
    }
    virtual void mouseDoubleClickEvent(QMouseEvent *event)
    {
        if(displayType != TimeLabel::Both) return;
        event->accept();
        toggleTimeDisplay();
        ClickableQLabel::mouseDoubleClickEvent(event);
    }

private:
    bool b_remainingTime;
    int cachedLength;
    QTimer *bufTimer;

    bool buffering;
    bool showBuffering;
    float bufVal;
    TimeLabel::Display displayType;

    char psz_length[MSTRTIME_MAX_SIZE];
    char psz_time[MSTRTIME_MAX_SIZE];
    void toggleTimeDisplay();
    void paintEvent(QPaintEvent*);

public slots:
    void setDisplayPosition(float pos, qint64 time, qint64 length);
    void setDisplayPosition(float pos);
    void updateBuffering(float);
    void updateBuffering();
};

#endif // TIMELABEL_H
