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

#ifndef QTVLC_WIDGETS_SPECIALBUTTONS_H
#define QTVLC_WIDGETS_SPECIALBUTTONS_H

#include <QtWidgets/QToolButton>

class PlayButton : public QToolButton
{
    Q_OBJECT
public:
    PlayButton(QWidget *parent = 0) : QToolButton(parent) { updateButtonIcons(false); }
public Q_SLOTS:
    void updateButtonIcons(bool);
};

class LoopButton : public QToolButton
{
    Q_OBJECT
public:
    LoopButton(QWidget *parent = 0) : QToolButton(parent) { updateButtonIcons(0); }
public Q_SLOTS:
    void updateButtonIcons(int);
};

class AtoB_Button : public QToolButton
{
    Q_OBJECT
public:
    AtoB_Button(QWidget *parent = 0) : QToolButton(parent) {}
public Q_SLOTS:
    void updateButtonIcons(bool, bool);
};

#endif // QTVLC_WIDGETS_SPECIALBUTTONS_H
