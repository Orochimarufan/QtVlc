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

#include "VlcMediaPlayerVideo.h"
#include "VlcMediaPlayer.h"
#include "VlcError.h"

#include "VlcMediaPlayer_p.h"

// validity
bool VlcMediaPlayerVideo::isValid()
{
    return d != nullptr;
}

// NULL constructor
VlcMediaPlayerVideo::VlcMediaPlayerVideo() :
    QObject(), d(nullptr)
{
}

// constructor/assign
VlcMediaPlayerVideo::VlcMediaPlayerVideo(const VlcMediaPlayerVideo &o) :
    QObject(), d(o.d)
{
    if (d != nullptr)
        d->retain();
}

VlcMediaPlayerVideo &VlcMediaPlayerVideo::operator =(const VlcMediaPlayerVideo &o)
{
    if (d != nullptr)
        d->release();

    d = o.d;

    if (d != nullptr)
        d->retain();

    return *this;
}

VlcMediaPlayerVideo::VlcMediaPlayerVideo(const VlcMediaPlayer &player) :
    QObject()
{
    d = VlcMediaPlayerPrivate::instance(getref<VlcMediaPlayer>(player)->data());
}

VlcMediaPlayerVideo &VlcMediaPlayerVideo::operator =(const VlcMediaPlayer &player)
{
    if (d != nullptr)
        d->release();

    d = VlcMediaPlayerPrivate::instance(getref<VlcMediaPlayer>(player)->data());

    return *this;
}

VlcMediaPlayerVideo::VlcMediaPlayerVideo(libvlc_media_player_t *player) :
    QObject()
{
    d = VlcMediaPlayerPrivate::instance(player);
}

VlcMediaPlayerVideo &VlcMediaPlayerVideo::operator =(libvlc_media_player_t *player)
{
    if (d != nullptr)
        d->release();

    d = VlcMediaPlayerPrivate::instance(player);

    return *this;
}

VlcMediaPlayerVideo::VlcMediaPlayerVideo(VlcMediaPlayerPrivate *p) :
    QObject(), d(p)
{
    if (d != nullptr)
        d->retain();
}

// destructor
VlcMediaPlayerVideo::~VlcMediaPlayerVideo()
{
    if (d != nullptr)
        d->release();
}

// API
bool VlcMediaPlayerVideo::getSize(unsigned num, unsigned int *x, unsigned int *y) const
{
    CHECKNP return d->video_getSize(num, x, y);
}

bool VlcMediaPlayerVideo::getSize(unsigned int *x, unsigned int *y) const
{
    CHECKNP return d->video_getSize(0, x, y);
}

bool VlcMediaPlayerVideo::getCursor(unsigned num, int *x, int *y) const
{
    CHECKNP return d->video_getCursor(num, x, y);
}

bool VlcMediaPlayerVideo::getCursor(int *x, int *y) const
{
    CHECKNP return d->video_getCursor(0, x, y);
}

float VlcMediaPlayerVideo::scale() const
{
    CHECKNP return d->video_scale();
}

QString VlcMediaPlayerVideo::aspectRatio() const
{
    CHECKNP return d->video_aspectRatio();
}

int VlcMediaPlayerVideo::spu() const
{
    CHECKNP return d->video_spu();
}

int VlcMediaPlayerVideo::spuCount() const
{
    CHECKNP return d->video_spuCount();
}

QHash<int, QString> VlcMediaPlayerVideo::spuDescription() const
{
    CHECKNP return d->video_spuDescription();
}

qint64 VlcMediaPlayerVideo::spuDelay() const
{
    CHECKNP return d->video_spuDelay();
}

QHash<int, QString> VlcMediaPlayerVideo::titleDescription() const
{
    CHECKNP return d->video_titleDescription();
}

QHash<int, QString> VlcMediaPlayerVideo::chapterDescription(int title) const
{
    CHECKNP return d->video_chapterDescription(title);
}

QString VlcMediaPlayerVideo::cropGeometry() const
{
    CHECKNP return d->video_cropGeometry();
}

int VlcMediaPlayerVideo::teletext() const
{
    CHECKNP return d->video_teletext();
}

int VlcMediaPlayerVideo::trackCount() const
{
    CHECKNP return d->video_trackCount();
}

QHash<int, QString> VlcMediaPlayerVideo::trackDescription() const
{
    CHECKNP return d->video_trackDescription();
}

int VlcMediaPlayerVideo::track() const
{
    CHECKNP return d->video_track();
}

int VlcMediaPlayerVideo::marqueeInt(unsigned option) const
{
    CHECKNP return d->video_marqueeInt(option);
}

QString VlcMediaPlayerVideo::marqueeString(unsigned option) const
{
    CHECKNP return d->video_marqueeString(option);
}

int VlcMediaPlayerVideo::logoInt(unsigned option) const
{
    CHECKNP return d->video_logoInt(option);
}

int VlcMediaPlayerVideo::adjustInt(unsigned option) const
{
    CHECKNP return d->video_adjustInt(option);
}

float VlcMediaPlayerVideo::adjustFloat(unsigned option) const
{
    CHECKNP return d->video_adjustFloat(option);
}

// slots
void VlcMediaPlayerVideo::setKeyInput(bool on)
{
    CHECKNP d->video_setKeyInput(on);
}

void VlcMediaPlayerVideo::setMouseInput(bool on)
{
    CHECKNP d->video_setMouseInput(on);
}

void VlcMediaPlayerVideo::setScale(float scale)
{
    CHECKNP d->video_setScale(scale);
}

void VlcMediaPlayerVideo::setAspectRatio(const QString &ratio)
{
    CHECKNP d->video_setAspectRatio(ratio);
}

bool VlcMediaPlayerVideo::setSpu(int spu)
{
    CHECKNP return d->video_setSpu(spu);
}

bool VlcMediaPlayerVideo::setSubtitleFile(const QString &filename)
{
    CHECKNP return d->video_setSubtitleFile(filename);
}

bool VlcMediaPlayerVideo::setSpuDelay(qint64 delay)
{
    CHECKNP return d->video_setSpuDelay(delay);
}

void VlcMediaPlayerVideo::setCropGeometry(const QString &geometry)
{
    CHECKNP d->video_setCropGeometry(geometry);
}

void VlcMediaPlayerVideo::setTeletext(int page)
{
    CHECKNP d->video_setTeletext(page);
}

void VlcMediaPlayerVideo::toggleTeletext()
{
    CHECKNP d->video_toggleTeletext();
}

bool VlcMediaPlayerVideo::setTrack(int track)
{
    CHECKNP return d->video_setTrack(track);
}

bool VlcMediaPlayerVideo::takeSnapshot(unsigned num, const QString &filename, unsigned int width, unsigned int height)
{
    CHECKNP return d->video_takeSnapshot(num, filename, width, height);
}

bool VlcMediaPlayerVideo::takeSnapshot(const QString &filename, unsigned int width, unsigned int height)
{
    CHECKNP return d->video_takeSnapshot(0, filename, width, height);
}

bool VlcMediaPlayerVideo::takeSnapshot(const QString &filename)
{
    CHECKNP
    unsigned int x, y;
    d->video_getSize(0, &x, &y);
    return d->video_takeSnapshot(0, filename, x, y);
}

void VlcMediaPlayerVideo::setDeinterlace(const QString &mode)
{
    CHECKNP d->video_setDeinterlace(mode);
}

void VlcMediaPlayerVideo::setMarqueeInt(unsigned option, int value)
{
    CHECKNP d->video_setMarqueeInt(option, value);
}

void VlcMediaPlayerVideo::setMarqueeString(unsigned option, const QString &text)
{
    CHECKNP d->video_setMarqueeString(option, text);
}

void VlcMediaPlayerVideo::setLogoInt(unsigned option, int value)
{
    CHECKNP d->video_setLogoInt(option, value);
}

void VlcMediaPlayerVideo::setLogoString(unsigned option, const QString &value)
{
    CHECKNP d->video_setLogoString(option, value);
}

void VlcMediaPlayerVideo::setAdjustInt(unsigned option, int value)
{
    CHECKNP d->video_setAdjustInt(option, value);
}

void VlcMediaPlayerVideo::setAdjustFloat(unsigned option, float value)
{
    CHECKNP d->video_setAdjustFloat(option, value);
}
