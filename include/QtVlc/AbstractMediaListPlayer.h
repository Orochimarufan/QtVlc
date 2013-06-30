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

#ifndef QTVLC_ABSTRACTMEDIALISTPLAYER_H
#define QTVLC_ABSTRACTMEDIALISTPLAYER_H

#include <QtCore/QObject>

#include <QtVlc/enum.h>

class VlcMediaPlayer;
class AbstractMediaList;
class VlcMedia;

class AbstractMediaListPlayer : public QObject
{
    Q_OBJECT
public:
    virtual ~AbstractMediaListPlayer();

    // ***** Virtual *****
    virtual void setMediaPlayer(const VlcMediaPlayer &player) = 0;
    virtual void setMediaList(const AbstractMediaList &list) = 0;

    virtual bool isPlaying() = 0;
    virtual VlcState::Type state() = 0;

public Q_SLOTS:
    virtual void play() = 0;
    virtual void play(const VlcMedia &media) = 0;
    virtual void play(int index) = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void next() = 0;
    virtual void previous() = 0;
    virtual void setPlaybackMode(const VlcPlaybackMode::Type &mode) = 0;
};

#endif // QTVLC_ABSTRACTMEDIALISTPLAYER_H
