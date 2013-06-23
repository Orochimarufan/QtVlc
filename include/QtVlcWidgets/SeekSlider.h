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
 * input_slider.hpp : VolumeSlider and SeekSlider
 ****************************************************************************
 * Copyright (C) 2006-2011 the VideoLAN team
 * $Id: 00b7b19daf5c48a10936a4d8b8943f1d2f8b48ae $
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *          Ludovic Fauvet <etix@videolan.org>
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef QTVLC_WIDGETS_SEEKSLIDER_H
#define QTVLC_WIDGETS_SEEKSLIDER_H

#include <QtWidgets/QSlider>

#include <QtVlcWidgets/config.h>
#include <QtVlcWidgets/const.h>
#include <QtVlcWidgets/TimeTooltip.h>

class QMouseEvent;
class QWheelEvent;
class QHideEvent;
class QTimer;
//class SeekPoints;
class QPropertyAnimation;
class QStyleOption;
class QCommonStyle;

/* Input Slider derived from QSlider */
class QtVlcWidgets_EXPORT SeekSlider : public QSlider
{
    Q_OBJECT
    Q_PROPERTY(qreal handleOpacity READ handleOpacity WRITE setHandleOpacity)
public:
    SeekSlider(QWidget *_parent = 0) : SeekSlider(Qt::Horizontal, _parent) {}
    SeekSlider(Qt::Orientation q, QWidget *_parent = 0, bool _classic = false);
    ~SeekSlider();
    //void setChapters(SeekPoints *);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    virtual void hideEvent(QHideEvent *);

    virtual bool eventFilter(QObject *obj, QEvent *event);

    virtual QSize sizeHint() const;

    void processReleasedButton();
    bool isAnimationRunning() const;
    qreal handleOpacity() const;
    void setHandleOpacity(qreal opacity);
    int handleLength();

private:
    bool isSliding;        /* Whether we are currently sliding by user action */
    bool isJumping;              /* if we requested a jump to another chapter */
    int inputLength;                           /* InputLength that can change */
    char psz_length[MSTRTIME_MAX_SIZE];               /* Used for the ToolTip */
    QTimer *seekLimitTimer;
    TimeTooltip *mTimeTooltip;
    float f_buffering;
    //SeekPoints* chapters;
    bool b_classic;
    bool b_seekable;
    int mHandleLength;

    /* Colors & gradients */
    QSize gradientsTargetSize;
    QLinearGradient backgroundGradient;
    QLinearGradient foregroundGradient;
    QLinearGradient handleGradient;
    QColor tickpointForeground;
    QColor shadowDark;
    QColor shadowLight;
    QCommonStyle *alternativeStyle;

    /* Handle's animation */
    qreal mHandleOpacity;
    QPropertyAnimation *animHandle;
    QTimer *hideHandleTimer;

public slots:
    void setPosition(float, qint64, qint64);
    void setSeekable(bool b) { b_seekable = b ; }
    void updateBuffering(float);
    void hideHandle();

private slots:
    void startSeekTimer();
    void updatePos();

signals:
    void sliderDragged(float);

    friend class SeekStyle;
};

#endif // QTVLC_WIDGETS_SEEKSLIDER_H
