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

#ifndef QTVLC_ABSTRACTMEDIALIST_H
#define QTVLC_ABSTRACTMEDIALIST_H

#include <QtCore/QObject>

#include <QtVlc/config.h>

class VlcMedia;

class QtVlc_EXPORT AbstractMediaList : public QObject
{
    Q_OBJECT

public:
    virtual ~AbstractMediaList() {}

    // ***** Virtual *****
    // read methods
    virtual int length() const  = 0;
    virtual VlcMedia at(int index) = 0;
    virtual int indexOf(const VlcMedia &media, int from = 0) = 0;

    // write methods
    virtual bool isReadOnly() = 0;
    virtual void insert(int index, const VlcMedia &media) = 0;
    virtual void removeAt(int index) = 0;

    // optional methods
    virtual void append(const VlcMedia &media);
    virtual void prepend(const VlcMedia &media);

    virtual bool isEmpty();
    virtual bool contains(const VlcMedia &media);

    virtual VlcMedia takeAt(int index);
    virtual VlcMedia takeFirst();
    virtual VlcMedia takeLast();

    virtual VlcMedia first();
    virtual VlcMedia last();

    virtual void removeFirst();
    virtual void removeLast();
    virtual void removeOne(const VlcMedia &media);
    virtual int removeAll(const VlcMedia &media);

    virtual void replace(int index, const VlcMedia &new_media);
    virtual void swap(int i, int j);
    virtual void move(int from, int to);

    // ***** Aliases *****
    VlcMedia operator [](int);
    AbstractMediaList &operator <<(const VlcMedia &);
    AbstractMediaList &operator +=(const VlcMedia &);

    VlcMedia value(int index);
    VlcMedia value(int index, const VlcMedia &defaultValue);

    int size() const;
    int count() const;

    bool startsWith(const VlcMedia &);
    bool endsWith(const VlcMedia &);

Q_SIGNALS:
    void itemAboutToBeAdded(const VlcMedia &, int);
    void itemAdded(const VlcMedia &, int);
    void itemAboutToBeRemoved(const VlcMedia &, int);
    void itemRemoved(const VlcMedia &, int);
};

class AbstractReadOnlyMediaList : public AbstractMediaList
{
    Q_OBJECT
public:
    virtual bool isReadOnly();
    virtual void insert(int index, const VlcMedia &media);
    virtual void removeAt(int index);
};

struct ReadOnly {};

#endif // QTVLC_ABSTRACTMEDIALIST_H
