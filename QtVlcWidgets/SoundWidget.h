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
 * controller_widget.hpp : Controller Widget for the controllers
 ****************************************************************************
 * Copyright (C) 2006-2008 the VideoLAN team
 * $Id: 48a6d96b098aa5594d75e8bbba3b3f143f90f101 $
 *
 * Authors: Jean-Baptiste Kempf <jb@videolan.org>
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

#ifndef QTVLC_WIDGETS_SOUNDWIDGET_H
#define QTVLC_WIDGETS_SOUNDWIDGET_H

#include <QtWidgets/QWidget>
#include "QtVlcWidgetsConfig.h"

class QLabel;
class QAbstractSlider;
class QFrame;
class QMenu;

class QtVlcWidgets_EXPORT SoundWidget : public QWidget
{
    Q_OBJECT

public:
    SoundWidget(QWidget *parent, bool b_fancy = true, bool b_special = false);
    virtual ~SoundWidget();
    void setMuted(bool);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *e);

private:
    QLabel              *volMuteLabel;
    QAbstractSlider     *volumeSlider;
    QFrame              *volumeControlWidget;
    QMenu               *volumeMenu;

    bool                b_is_muted;
    bool                b_ignore_valuechanged;

public Q_SLOTS:
    void updateVolume(int);

protected Q_SLOTS:
    void userUpdateVolume(int);
    void updateMuteStatus(bool);
    void refreshLabels(void);
    void showVolumeMenu(QPoint pos);
    void valueChangedFilter(int);

Q_SIGNALS:
    void volumeChanged(int);
    void muteChanged(bool);
};

#endif // QTVLC_WIDGETS_SOUNDWIDGET_H
