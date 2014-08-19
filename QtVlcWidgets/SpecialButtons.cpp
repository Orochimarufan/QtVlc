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
 * controller_widget.cpp : Controller Widget for the controllers
 ****************************************************************************
 * Copyright (C) 2006-2008 the VideoLAN team
 * $Id: 1b3f4782dc3620531ee6eb25ea43ef782047a372 $
 *
 * Authors: Jean-Baptiste Kempf <jb@videolan.org>
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

#include "SpecialButtons.h"

void PlayButton::updateButtonIcons( bool b_playing )
{
    setIcon( b_playing ? QIcon( ":/toolbar/pause_b" ) : QIcon( ":/toolbar/play_b" ) );
    setToolTip( b_playing ? tr( "Pause the playback" )
                          : tr( "Start playback" ) );
}

void AtoB_Button::updateButtonIcons( bool timeA, bool timeB )
{
    if( !timeA && !timeB)
    {
        setIcon( QIcon( ":/toolbar/atob_nob" ) );
        setToolTip( tr( "Loop from point A to point B continuously\n"
                         "Click to set point A" ) );
    }
    else if( timeA && !timeB )
    {
        setIcon( QIcon( ":/toolbar/atob_noa" ) );
        setToolTip( tr( "Click to set point B" ) );
    }
    else if( timeA && timeB )
    {
        setIcon( QIcon( ":/toolbar/atob" ) );
        setToolTip( tr( "Stop the A to B loop" ) );
    }
}

void LoopButton::updateButtonIcons( int value )
{
    setChecked( value != LB_NORMAL );
    setIcon( ( value == LB_REPEAT_ONE ) ? QIcon( ":/buttons/playlist/repeat_one" )
                                     : QIcon( ":/buttons/playlist/repeat_all" ) );
}
