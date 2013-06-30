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

#include <QtVlc/AbstractMediaList.h>
#include <QtVlc/VlcMedia.h>

AbstractMediaList::~AbstractMediaList()
{}

// optional
void AbstractMediaList::append(const VlcMedia &media)
{
    insert(length(), media);
}

void AbstractMediaList::prepend(const VlcMedia &media)
{
    insert(0, media);
}


bool AbstractMediaList::isEmpty()
{
    return length() == 0;
}

bool AbstractMediaList::contains(const VlcMedia &media)
{
    return indexOf(media) != 0;
}


VlcMedia AbstractMediaList::takeAt(int index)
{
    VlcMedia item = value(index);
    removeAt(index);
    return item;
}

VlcMedia AbstractMediaList::takeFirst()
{
    VlcMedia item = first();
    removeFirst();
    return item;
}

VlcMedia AbstractMediaList::takeLast()
{
    VlcMedia item = last();
    removeLast();
    return item;
}


VlcMedia AbstractMediaList::first()
{
    return value(0);
}

VlcMedia AbstractMediaList::last()
{
    return value(length() -1);
}


void AbstractMediaList::removeFirst()
{
    removeAt(0);
}

void AbstractMediaList::removeLast()
{
    removeAt(length() - 1);
}

void AbstractMediaList::removeOne(const VlcMedia &media)
{
    removeAt(indexOf(media));
}

int AbstractMediaList::removeAll(const VlcMedia &media)
{
    int i, j = 0;
    while ((i = indexOf(media)) != -1)
    {
        removeAt(i);
        j++;
    }
    return j;
}


void AbstractMediaList::replace(int index, const VlcMedia &new_media)
{
    removeAt(index);
    insert(index, new_media);
}

void AbstractMediaList::swap(int i, int j)
{
    // [0, 1, 2, 3, 4]
    if (i < j) { // swap(1, 3)
        move(i, j - 1); // 1 -> (3 - 1 = 2) [0, 2, 1, 3, 4]
        move(j, i); // 3 -> 1 [0, 3, 2, 1, 4]
    } else { // swap(3, 1)
        move(j, i - 1); // as above, i and j swapped
        move(i, j);
    }
}

void AbstractMediaList::move(int from, int to)
{
    insert(to, takeAt(from));
}


// aliases
VlcMedia AbstractMediaList::operator [](int index)
{
    return value(index);
}

AbstractMediaList &AbstractMediaList::operator <<(const VlcMedia &media)
{
    append(media);
    return *this;
}

AbstractMediaList &AbstractMediaList::operator +=(const VlcMedia &media)
{
    append(media);
    return *this;
}


VlcMedia AbstractMediaList::value(int index)
{
    if (index >= length() || index < 0)
        return VlcMedia(nullptr);
    else
        return at(index);
}

VlcMedia AbstractMediaList::value(int index, const VlcMedia &defaultValue)
{
    if (index >= length() || index < 0)
        return defaultValue;
    else
        return at(index);
}

int AbstractMediaList::size() const
{
    return length();
}

int AbstractMediaList::count() const
{
    return length();
}

bool AbstractMediaList::startsWith(const VlcMedia &media)
{
    return first() == media;
}

bool AbstractMediaList::endsWith(const VlcMedia &media)
{
    return last() == media;
}


// read only
bool AbstractReadOnlyMediaList::isReadOnly()
{
    return true;
}

void AbstractReadOnlyMediaList::insert(int index, const VlcMedia &media)
{
    throw new ReadOnly;
}

void AbstractReadOnlyMediaList::removeAt(int index)
{
    throw new ReadOnly;
}
