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

#ifndef QTVLC_VLCINSTANCE_H
#define QTVLC_VLCINSTANCE_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include "QtVlcConfig.h"

struct libvlc_instance_t;
class VlcInstancePrivate;

/**
 * @brief The VlcInstance class
 * The master mind ;D
 */
class QtVlc_EXPORT VlcInstance : public QObject
{
    Q_OBJECT
    VlcInstancePrivate *d;

public:
    /**
     * @brief Check if this VlcInstance is valid
     * True if this VlcInstance has underlying data.
     * Most methods will throw a NullPointer exception if called invalid objects.
     */
    bool isValid();

    // copy, assignment & libvlc primitive
    VlcInstance(const VlcInstance &);
    VlcInstance &operator =(const VlcInstance &);
    VlcInstance(libvlc_instance_t *);
    VlcInstance &operator =(libvlc_instance_t *);
    libvlc_instance_t *data(); // refcount is NOT increased!

    /**
     * @brief VlcInstance constructor
     * @param args the libvlc commandline arguments
     */
    explicit VlcInstance(const QStringList &args);

    // destructor
    virtual ~VlcInstance();

    // ----------- Options -----------
    /**
     * @brief Sets the application name.
     * @param application human-readable application name, e.g. "The QtVlc Demo 1.2.3"
     * @param version HTTP User Agent, e.g. "QtVlc-Demo/1.2.3"
     */
    void setUserAgent(const QString &application, const QString &http);

    /**
     * @brief Sets some meta-information about the application
     * @param id Java-style application identifier, e.g. "me.sodimm.oro.qtvlc.testapp"
     * @param version application version numbers, e.g. "1.2.3"
     * @param icon_name application icon name, e.g. "video-player"
     * @since libvlc 2.1.0
     * TODO: stack backward-compatible with older libvlc while providing this method?
     */
    //void setAppId(const QString &id, const QString &version, const QString &icon_name);



    // ----------- Version -----------
    static QString libvlc_version();
    static QString libvlc_compiler();
    static QString libvlc_changeset();

    static QString QtVlc_version();
    static QString QtVlc_version_git();
    static QString QtVlc_build_libvlc_version();
    static QString QtVlc_build_qt_version();

    // ----------- Global Instance -----------
    /**
     * @brief VlcInstance constructor
     * The global VlcInstance
     */
    VlcInstance();

    /**
     * @brief The raw global instance
     */
    static libvlc_instance_t *globalInstance();

    /**
     * @brief Initialize the global default Instance
     * @param args The libvlc_new arguments
     * @return success (fails if a global instance exists already)
     */
    static bool initGlobalInstance(const QStringList &args);

    /**
     * @brief Drop the global default Instance
     * This will not delete the instance if it is still referenced.
     */
    static void freeGlobalInstance();
};

#endif // QTVLC_VLCINSTANCE_H
