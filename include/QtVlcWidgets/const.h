/*****************************************************************************
 * QtVlc - C++ bindings for libVLC using Qt awesomeness
 * Copyright (C) 2013 Orochimarufan <orochimarufan.x3@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifndef QTVLC_WIDGETS_CONST_H
#define QTVLC_WIDGETS_CONST_H

// Time Strings
#define MSTRTIME_MAX_SIZE 22

// SoundSlider
#define WLENGTH   80 // px
#define WHEIGHT   22  // px
#define SOUNDMIN  0   // %
#define SOUNDMAX  125 // % (+6dB)
#define SOUND_SLIDER_COLORS QStringLiteral("1;66;2;77;3;99;4;33;5;22;6;97")

// SeekSlider
#define MINIMUM 0
#define MAXIMUM 1000
//#define CHAPTERSSPOTSIZE 3
#define FADEDURATION 300
#define FADEOUTDELAY 2000

#endif // CONST_H
