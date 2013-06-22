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
 * GNU Lesser General Public License for more details.
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
 * ( at your option ) any later version.
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

#include <QtCore/QVariant>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QBitmap>

#ifdef Q_WS_X11
#   include <qx11info_x11.h>
#   include <X11/Xlib.h>
#endif

#include <cassert>

#include "QtVlcWidgets/VlcVideoWidget.h"


VlcVideoWidget::VlcVideoWidget(QWidget *parent, QWidget *dw) :
    QFrame(parent), stable(nullptr), default_widget(dw)
{
    /* Set the policy to expand in both directions */
    // setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    if (default_widget)
        layout->addWidget(default_widget);

    show();
}

VlcVideoWidget::~VlcVideoWidget()
{
    /* Ensure we are not leaking the video output. This would crash. */
    assert(!stable);
}

void VlcVideoWidget::sync()
{
#ifdef Q_WS_X11
    /* Make sure the X server has processed all requests.
     * This protects other threads using distinct connections from getting
     * the video widget window in an inconsistent states. */
    XSync( QX11Info::display(), False );
#endif
}

/**
 * Request the video to avoid the conflicts
 **/
WId VlcVideoWidget::request(bool b_keep_size, unsigned int width, unsigned int height)
{
    // TODO sizing
    qDebug("Video was requested %i, %i", width, height );

    if(stable)
    {
        qDebug("embedded video already in use");
        return 0;
    }

    /* The owner of the video window needs a stable handle (WinId). Reparenting
     * in Qt4-X11 changes the WinId of the widget, so we need to create another
     * dummy widget that stays within the reparentable widget. */
    stable = new QWidget();
    QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::black);
    stable->setPalette(plt);
    stable->setAutoFillBackground(true);
    /* Force the widget to be native so that it gets a winId() */
    stable->setAttribute(Qt::WA_NativeWindow, true);
    /* Indicates that the widget wants to draw directly onto the screen.
       Widgets with this attribute set do not participate in composition
       management */
    /* This is currently disabled on X11 as it does not seem to improve
     * performance, but causes the video widget to be transparent... */
#if !defined (Q_WS_X11) && !defined (Q_WS_QPA)
    stable->setAttribute(Qt::WA_PaintOnScreen, true);
#endif

    if (default_widget)
        layout->removeWidget(default_widget);
    layout->addWidget(stable);

#ifdef Q_WS_X11
    /* HACK: Only one X11 client can subscribe to mouse button press events.
     * VLC currently handles those in the video display.
     * Force Qt4 to unsubscribe from mouse press and release events. */
    /*Display *dpy = QX11Info::display();
    Window w = stable->winId();
    XWindowAttributes attr;

    XGetWindowAttributes( dpy, w, &attr );
    attr.your_event_mask &= ~(ButtonPressMask|ButtonReleaseMask);
    XSelectInput( dpy, w, attr.your_event_mask );*/
#endif
    sync();
    return stable->winId();
}

void VlcVideoWidget::setSize(unsigned int w, unsigned int h)
{
    resize(w, h);
    emit sizeChanged(w, h);

    if ((unsigned)size().width() == w && (unsigned)size().height() == h)
        updateGeometry();

    sync();
}

void VlcVideoWidget::release()
{
    qDebug("Video is no longer needed.");

    if (stable)
    {
        layout->removeWidget(stable);
        stable->deleteLater();
        stable = nullptr;
        if (default_widget)
            layout->addWidget(default_widget);
    }

    updateGeometry();
}


// Background Widget
#define __MIN(a,b) ((a)>(b)?a:b)
#define MARGIN 10

VlcPrimitiveBackgroundWidget::VlcPrimitiveBackgroundWidget(QString path, QWidget *parent) :
    QWidget(parent), _path(path)
{
    setAutoFillBackground(true);
    QPalette plt = palette();
    plt.setColor(QPalette::Active, QPalette::Window, Qt::black);
    plt.setColor(QPalette::Inactive, QPalette::Window, Qt::black);
    setPalette(plt);
}

VlcPrimitiveBackgroundWidget::~VlcPrimitiveBackgroundWidget()
{}

void VlcPrimitiveBackgroundWidget::setPath(QString path)
{
    _path = path;
}

void VlcPrimitiveBackgroundWidget::paintEvent(QPaintEvent *e)
{
    int i_maxwidth, i_maxheight;
    QPixmap pixmap = QPixmap(_path);
    QPainter painter(this);
    QBitmap pMask;
    float f_alpha = 1.0;

    i_maxwidth  = __MIN(maximumWidth(), width()) - MARGIN * 2;
    i_maxheight = __MIN(maximumHeight(), height()) - MARGIN * 2;

    painter.setOpacity(property("opacity").toFloat());

    if (height() > MARGIN * 2)
    {
        /* Scale down the pixmap if the widget is too small */
        if (pixmap.width() > i_maxwidth || pixmap.height() > i_maxheight)
        {
            pixmap = pixmap.scaled(i_maxwidth, i_maxheight,
                            Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        else
        if (b_expandPixmap &&
            pixmap.width() < width() && pixmap.height() < height())
        {
            /* Scale up the pixmap to fill widget's size */
            f_alpha = ((float) pixmap.height() / (float) height());
            pixmap = pixmap.scaled(
                    width() - MARGIN * 2,
                    height() - MARGIN * 2,
                    Qt::KeepAspectRatio,
                    (f_alpha < .2)? /* Don't waste cpu when not visible */
                        Qt::SmoothTransformation:
                        Qt::FastTransformation
                    );
            /* Non agressive alpha compositing when sizing up */
            pMask = QBitmap(pixmap.width(), pixmap.height());
            pMask.fill(QColor::fromRgbF(1.0, 1.0, 1.0, f_alpha));
            pixmap.setMask(pMask);
        }

        painter.drawPixmap(
                MARGIN + (i_maxwidth - pixmap.width()) /2,
                MARGIN + (i_maxheight - pixmap.height()) /2,
                pixmap);
    }
    QWidget::paintEvent( e );
}
