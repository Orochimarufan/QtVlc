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

#include "VlcMediaListPlayer.h"
#include "VlcError.h"
#include "VlcInstance.h"
#include "VlcMediaList.h"
#include "VlcMediaPlayer.h"
#include "VlcMedia.h"

#include "VlcMediaListPlayer_p.h"

void VlcMediaListPlayer::d_connect()
{
    connect(d, SIGNAL(played()), SIGNAL(played()));
    connect(d, SIGNAL(nextItemSet(VlcMedia)), SIGNAL(nextItemSet(VlcMedia)));
    connect(d, SIGNAL(stopped()), SIGNAL(stopped()));
}

bool VlcMediaListPlayer::isValid()
{
    return d != nullptr;
}

VlcMediaListPlayer::VlcMediaListPlayer(const VlcMediaListPlayer &o) :
    AbstractMediaListPlayer(), d(o.d)
{
    if (d != nullptr)
    {
        d->retain();
        d_connect();
    }
}

VlcMediaListPlayer &VlcMediaListPlayer::operator =(const VlcMediaListPlayer &o)
{
    if (d != nullptr)
    {
        disconnect(d);
        d->release();
    }

    d = o.d;

    if (d != nullptr)
    {
        d->retain();
        d_connect();
    }
    
    return *this;
}

VlcMediaListPlayer::VlcMediaListPlayer(libvlc_media_list_player_t *p_mlp) :
    AbstractMediaListPlayer()
{
    d = VlcMediaListPlayerPrivate::instance(p_mlp);

    if (d != nullptr)
        d_connect();
}

VlcMediaListPlayer &VlcMediaListPlayer::operator =(libvlc_media_list_player_t *p_mlp)
{
    if (d != nullptr)
    {
        disconnect(d);
        d->release();
    }

    d = VlcMediaListPlayerPrivate::instance(p_mlp);

    if (d != nullptr)
        d_connect();
    
    return *this;
}

libvlc_media_list_player_t *VlcMediaListPlayer::data()
{
    CHECKNP
    return d->data();
}

VlcMediaListPlayer::~VlcMediaListPlayer()
{
    if (d != nullptr)
        d->release();
}

VlcMediaListPlayer::VlcMediaListPlayer(const VlcInstance &p_i) :
    AbstractMediaListPlayer()
{
    d = new VlcMediaListPlayerPrivate(getref<VlcInstance>(p_i)->data());

    d_connect();
}

VlcMediaListPlayer::VlcMediaListPlayer(libvlc_instance_t *p_i) :
    AbstractMediaListPlayer()
{
    d = new VlcMediaListPlayerPrivate(p_i);

    d_connect();
}

VlcMediaListPlayer::VlcMediaListPlayer() :
    AbstractMediaListPlayer()
{
    d = new VlcMediaListPlayerPrivate(VlcInstance::globalInstance());

    d_connect();
}

void VlcMediaListPlayer::setMediaPlayer(const VlcMediaPlayer &player)
{
    CHECKNP
    d->setMediaPlayer(getref<VlcMediaPlayer>(player)->data());
}

void VlcMediaListPlayer::setMediaList(const AbstractMediaList &list)
{
    CHECKNP
    VlcMediaList *l = qobject_cast<VlcMediaList *>(getref<AbstractMediaList>(list));
    if (l == 0)
        throw "VlcMediaListPlayer requires VlcMediaList instances!";
    d->setMediaList(l->data());
}

bool VlcMediaListPlayer::isPlaying()
{
    CHECKNP
    return d->isPlaying();
}

VlcState::Type VlcMediaListPlayer::state()
{
    CHECKNP
    return d->state();
}

void VlcMediaListPlayer::play()
{
    CHECKNP
    d->play();
}

void VlcMediaListPlayer::play(const VlcMedia &media)
{
    CHECKNP
    d->playItem(getref<VlcMedia>(media)->data());
}

void VlcMediaListPlayer::play(int index)
{
    CHECKNP
    d->playItemAtIndex(index);
}

void VlcMediaListPlayer::pause()
{
    CHECKNP
    d->pause();
}

void VlcMediaListPlayer::stop()
{
    CHECKNP
    d->stop();
}

void VlcMediaListPlayer::next()
{
    CHECKNP
    d->next();
}

void VlcMediaListPlayer::previous()
{
    CHECKNP
    d->previous();
}

void VlcMediaListPlayer::setPlaybackMode(const VlcPlaybackMode::Type &mode)
{
    CHECKNP
    d->setPlaybackMode(mode);
}
