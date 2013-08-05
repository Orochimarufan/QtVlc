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

#ifndef QTVLC_VLCMEDIAPLAYERVIDEO_H
#define QTVLC_VLCMEDIAPLAYERVIDEO_H

#include <QtCore/QObject>

#include "QtVlcConfig.h"

class VlcMediaPlayerPrivate;
class VlcMediaPlayer;
struct libvlc_media_player_t;

class QtVlc_EXPORT VlcMediaPlayerVideo : public QObject
{
    Q_OBJECT

    VlcMediaPlayerPrivate *d;

public:
    /**
     * @brief create a NULL VlcMediaPlayerVideo
     */
    VlcMediaPlayerVideo();

    /**
     * @brief Check if this VlcMediaPlayerVideo is valid
     * True if not constructed with VlcMediaPlayerVideo().
     * Most methods will throw a NullPointer exception if called invalid objects.
     * @return
     */
    bool isValid();

    VlcMediaPlayerVideo(const VlcMediaPlayerVideo &);
    VlcMediaPlayerVideo &operator =(const VlcMediaPlayerVideo &);
    VlcMediaPlayerVideo(libvlc_media_player_t *);
    VlcMediaPlayerVideo &operator =(libvlc_media_player_t *);
    VlcMediaPlayerVideo(const VlcMediaPlayer &);
    VlcMediaPlayerVideo &operator =(const VlcMediaPlayer &);
    VlcMediaPlayerVideo(VlcMediaPlayerPrivate *);

    virtual ~VlcMediaPlayerVideo();

    // API
    // TODO: documentation
    bool getSize(unsigned num, unsigned int *x, unsigned int *y) const;
    bool getSize(unsigned int *x, unsigned int *y) const;

    bool getCursor(unsigned num, int *x, int *y) const;
    bool getCursor(int *x, int *y) const;

    float scale() const;
    QString aspectRatio() const;

    int spu() const;
    int spuCount() const;
    QHash<int, QString> spuDescription() const;
    qint64 spuDelay() const;

    QHash<int, QString> titleDescription() const;
    QHash<int, QString> chapterDescription(int title) const;

    QString cropGeometry() const;

    int teletext() const;

    int trackCount() const;
    QHash<int, QString> trackDescription() const;
    int track() const;

    int marqueeInt(unsigned option) const;
    QString marqueeString(unsigned option) const;

    int logoInt(unsigned option) const;

    int adjustInt(unsigned option) const;
    float adjustFloat(unsigned option) const;

public Q_SLOTS:
    void setKeyInput(bool on);
    void setMouseInput(bool on);

    void setScale(float scale);
    void setAspectRatio(const QString &ratio);

    bool setSpu(int spu);
    bool setSubtitleFile(const QString &filename);
    bool setSpuDelay(qint64 delay);

    void setCropGeometry(const QString &geometry);

    void setTeletext(int page);
    void toggleTeletext();

    bool setTrack(int track);

    bool takeSnapshot(unsigned num, const QString &filename, unsigned int width, unsigned int height);
    bool takeSnapshot(const QString &filename, unsigned int width, unsigned int height);
    bool takeSnapshot(const QString &filename);

    void setDeinterlace(const QString &mode);

    void setMarqueeInt(unsigned option, int value);
    void setMarqueeString(unsigned option, const QString &text);

    void setLogoInt(unsigned option, int value);
    void setLogoString(unsigned option, const QString &value);

    void setAdjustInt(unsigned option, int value);
    void setAdjustFloat(unsigned option, float value);
};

#endif // QTVLC_VLCMEDIAPLAYERVIDEO_H
