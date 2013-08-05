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

#ifndef QTVLC_WIDGETS_SOUNDSLIDER_H
#define QTVLC_WIDGETS_SOUNDSLIDER_H

#include <QtWidgets/QAbstractSlider>
#include <QtGui/QPainter>

#include "QtVlcWidgetsConfig.h"
#include "QtVlcWidgetsConst.h"

/* Sound Slider inherited directly from QAbstractSlider */
class QtVlcWidgets_EXPORT SoundSlider : public QAbstractSlider
{
    Q_OBJECT
public:
    SoundSlider(QWidget *_parent) : SoundSlider(_parent, 1.0, SOUND_SLIDER_COLORS) {}
    SoundSlider(QWidget *_parent, float _i_step, QString psz_colors, int max = SOUNDMAX );
    void setMuted( bool ); /* Set Mute status */

public slots:
    void volumeChanged(const int &volume, const bool &muted)
    {
        setValue(volume);
        setMuted(muted);
    }

protected:
    const static int paddingL = 3;
    const static int paddingR = 2;

    virtual void paintEvent( QPaintEvent *);
    virtual void wheelEvent( QWheelEvent *event );
    virtual void mousePressEvent( QMouseEvent * );
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void mouseReleaseEvent( QMouseEvent * );

    void processReleasedButton();

private:
    bool isSliding; /* Whether we are currently sliding by user action */
    bool b_mouseOutside; /* Whether the mouse is outside or inside the Widget */
    int i_oldvalue; /* Store the old Value before changing */
    float f_step; /* How much do we increase each time we wheel */
    bool b_isMuted;

    QPixmap pixGradient; /* Gradient pix storage */
    QPixmap pixGradient2; /* Muted Gradient pix storage */
    QPixmap pixOutside; /* OutLine pix storage */
    QPainter painter;
    QColor background;
    QColor foreground;
    QFont textfont;
    QRect textrect;

    void changeValue( int x ); /* Function to modify the value from pixel x() */
};

#endif
