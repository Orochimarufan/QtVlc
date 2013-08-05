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
 * interface_widgets.cpp : Custom widgets for the main interface
 ****************************************************************************
 * Copyright (C) 2006-2010 the VideoLAN team
 * $Id: b22fb4396880ab3e45a3236919abbdafa5c1d622 $
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *          Rafaël Carré <funman@videolanorg>
 *          Ilkka Ollakka <ileoo@videolan.org>
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

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtGui/QPainter>

#include "TimeLabel.h"

#include "QtVlcWidgetsUtil.h"

#include <cstdio>


TimeLabel::TimeLabel(QWidget *parent, TimeLabel::Display _displayType)
    : ClickableQLabel(parent),  bufTimer(new QTimer(this)),
      buffering(false), showBuffering(false), bufVal(-1),
      displayType(_displayType), b_remainingTime(false)
{
    //if(_displayType != TimeLabel::Elapsed)
        //b_remainingTime = getSettings()->value("MainWindow/ShowRemainingTime", false).toBool();

    switch(_displayType) {
        case TimeLabel::Elapsed:
            setText(" --:-- ");
            setToolTip(tr("Elapsed time"));
            break;
        case TimeLabel::Remaining:
            setText(" --:-- ");
            setToolTip(tr("Total/Remaining time")
                        + QString("\n-")
                        + tr("Click to toggle between total and remaining time")
                     );
            break;
        case TimeLabel::Both:
            setText(" --:--/--:-- ");
            setToolTip(QString("- ")
                + tr("Click to toggle between elapsed and remaining time")
                + QString("\n- ")
                + tr("Double click to jump to a chosen time position"));
            break;
    }
    setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    bufTimer->setSingleShot(true);

    setStyleSheet("padding-left: 4px; padding-right: 4px;");
}

void TimeLabel::setDisplayPosition(float pos, qint64 t, qint64 l)
{
    showBuffering = false;
    bufTimer->stop();

    if(pos == -1.f)
    {
        setMinimumSize(QSize(0, 0));
        if(displayType == TimeLabel::Both)
            setText("--:--/--:--");
        else
            setText("--:--");
        return;
    }

    int time = t / 1000;
    int length = l / 1000;

    secstotimestr(psz_length, length);
    secstotimestr(psz_time, (b_remainingTime && length) ? length - time
                                                           : time);

    // compute the minimum size that will be required for the psz_length
    // and use it to enforce a minimal size to avoid "dancing" widgets
    QSize minsize(0, 0);
    if (length > 0)
    {
        QMargins margins = contentsMargins();
        minsize += QSize(
                  fontMetrics().size(0, QString(psz_length), 0, 0).width(),
                  sizeHint().height()
               );
        minsize += QSize(margins.left() + margins.right() + 8, 0); /* +padding */

        if (b_remainingTime)
            minsize += QSize(fontMetrics().size(0, "-", 0, 0).width(), 0);
    }

    switch(displayType)
    {
        case TimeLabel::Elapsed:
            setMinimumSize(minsize);
            setText(QString(psz_time));
            break;
        case TimeLabel::Remaining:
            if(b_remainingTime)
            {
                setMinimumSize(minsize);
                setText(QString("-") + QString(psz_time));
            }
            else
            {
                setMinimumSize(QSize(0, 0));
                setText(QString(psz_length));
            }
            break;
        case TimeLabel::Both:
        default:
            QString timestr = QString("%1%2/%3")
            .arg(QString((b_remainingTime && length) ? "-" : ""))
            .arg(QString(psz_time))
            .arg(QString((!length && time) ? "--:--" : psz_length));

            setText(timestr);
            break;
    }
    cachedLength = length;
}

void TimeLabel::setDisplayPosition(float pos)
{
    if(pos == -1.f || cachedLength == 0)
    {
        setText(" --:--/--:-- ");
        return;
    }

    int time = pos * cachedLength;
    secstotimestr(psz_time,
                   (b_remainingTime && cachedLength ?
                   cachedLength - time : time));
    QString timestr = QString("%1%2/%3")
        .arg(QString((b_remainingTime && cachedLength) ? "-" : ""))
        .arg(QString(psz_time))
        .arg(QString((!cachedLength && time) ? "--:--" : psz_length));

    setText(timestr);
}


void TimeLabel::toggleTimeDisplay()
{
    b_remainingTime = !b_remainingTime;
    //getSettings()->setValue("MainWindow/ShowRemainingTime", b_remainingTime);
}


void TimeLabel::updateBuffering(float _buffered)
{
    bufVal = _buffered;
    if(!buffering || bufVal == 0)
    {
        showBuffering = false;
        buffering = true;
        bufTimer->start(200);
    }
    else if(bufVal == 1)
    {
        showBuffering = buffering = false;
        bufTimer->stop();
    }
    update();
}

void TimeLabel::updateBuffering()
{
    showBuffering = true;
    update();
}

void TimeLabel::paintEvent(QPaintEvent* event)
{
    if(showBuffering)
    {
        QRect r(rect());
        r.setLeft(r.width() * bufVal);
        QPainter p(this);
        p.setOpacity(0.4);
        p.fillRect(r, palette().color(QPalette::Highlight));
    }
    QLabel::paintEvent(event);
}
