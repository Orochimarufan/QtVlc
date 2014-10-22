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

#include "SoundWidget.h"
#include "SoundSlider.h"
#include "QtVlcWidgetsConst.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMenu>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>


SoundWidget::SoundWidget(QWidget *_parent, bool b_shiny, bool b_special) :
    QWidget(_parent), b_is_muted(false), b_ignore_valuechanged(false)
{
    /* We need a layout for this widget */
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0); layout->setMargin(0);

    /* We need a Label for the pix */
    volMuteLabel = new QLabel;
    volMuteLabel->setPixmap(QPixmap(":/toolbar/volume-medium"));

    /* We might need a subLayout too */
    QVBoxLayout *subLayout;

    volMuteLabel->installEventFilter(this);

    /* Normal View, click on icon mutes */
    if(!b_special)
    {
        volumeMenu = NULL; subLayout = NULL;
        volumeControlWidget = NULL;

        /* And add the label */
        layout->addWidget(volMuteLabel, 0, Qt::AlignBottom);
    }
    else
    {
        /* Special view, click on button shows the slider */
        b_shiny = false;

        volumeControlWidget = new QFrame;
        subLayout = new QVBoxLayout(volumeControlWidget);
        subLayout->setContentsMargins(4, 4, 4, 4);
        volumeMenu = new QMenu(this);

        QWidgetAction *widgetAction = new QWidgetAction(volumeControlWidget);
        widgetAction->setDefaultWidget(volumeControlWidget);
        volumeMenu->addAction(widgetAction);

        /* And add the label */
        layout->addWidget(volMuteLabel);
    }

    /* Slider creation: shiny or clean */
    if(b_shiny)
    {
        volumeSlider = new SoundSlider(this
            //config_GetFloat(p_intf, "volume-step"),
            //var_InheritString(p_intf, "qt-slider-colours"),
            //var_InheritInteger(p_intf, "qt-max-volume")
            );
    }
    else
    {
        volumeSlider = new QSlider(NULL);
        volumeSlider->setAttribute(Qt::WA_MacSmallSize);
        volumeSlider->setOrientation(b_special ? Qt::Vertical
                                                : Qt::Horizontal);
        volumeSlider->setMaximum(200);
    }

    volumeSlider->setFocusPolicy(Qt::NoFocus);
    if(b_special)
        subLayout->addWidget(volumeSlider);
    else
        layout->addWidget(volumeSlider, 0, Qt::AlignBottom );

    /* Set the volume from the config */
    //float volume = playlist_VolumeGet(THEPL);
    //libUpdateVolume((volume >= 0.f) ? volume : 1.f);
    /* Sync mute status */
    //if(playlist_MuteGet(THEPL) > 0)
    //    updateMuteStatus(true);

    /* Volume control connection */
    volumeSlider->setTracking(true);
    //CONNECT(volumeSlider, valueChanged(int), this, valueChangedFilter(int));
    connect(volumeSlider, SIGNAL(valueChanged(int)), SLOT(valueChangedFilter(int)));
    //CONNECT(this, valueReallyChanged(int), this, userUpdateVolume(int));
    connect(this, SIGNAL(volumeChanged(int)), SLOT(userUpdateVolume(int)));
    //CONNECT(THEMIM, volumeChanged(float), this, libUpdateVolume(float));
    //CONNECT(THEMIM, soundMuteChanged(bool), this, updateMuteStatus(bool));
}

SoundWidget::~SoundWidget()
{
    delete volumeSlider;
    delete volumeControlWidget;
}

void SoundWidget::refreshLabels()
{
    int i_sliderVolume = volumeSlider->value();

    if(b_is_muted)
    {
        volMuteLabel->setPixmap(QPixmap(":/toolbar/volume-muted"));
        volMuteLabel->setToolTip(tr("Unmute"));
        return;
    }

    if(i_sliderVolume < SOUNDMAX / 3)
        volMuteLabel->setPixmap(QPixmap(":/toolbar/volume-low"));
    else if(i_sliderVolume > (SOUNDMAX * 2 / 3))
        volMuteLabel->setPixmap(QPixmap(":/toolbar/volume-high"));
    else volMuteLabel->setPixmap(QPixmap(":/toolbar/volume-medium"));
    volMuteLabel->setToolTip(tr("Mute"));
}

/* volumeSlider changed value event slot */
void SoundWidget::userUpdateVolume(int i_sliderVolume)
{
    Q_UNUSED(i_sliderVolume);
    
    /* Only if volume is set by user action on slider */
    setMuted(false);
    //playlist_VolumeSet(THEPL, i_sliderVolume / 100.f);
    refreshLabels();
}

/* libvlc changed value event slot */
void SoundWidget::updateVolume(int volume)
{
    if(volume != volumeSlider->value() )
    {
        b_ignore_valuechanged = true;
        volumeSlider->setValue(volume);
        b_ignore_valuechanged = false;
    }
    refreshLabels();
}

void SoundWidget::valueChangedFilter(int i_val)
{
    /* valueChanged is also emitted when the lib setValue() */
    if (!b_ignore_valuechanged) emit volumeChanged(i_val);
}

/* libvlc mute/unmute event slot */
void SoundWidget::updateMuteStatus(bool mute)
{
    b_is_muted = mute;

    SoundSlider *soundSlider = qobject_cast<SoundSlider *>(volumeSlider);
    if(soundSlider)
        soundSlider->setMuted(mute);
    refreshLabels();
}

void SoundWidget::showVolumeMenu(QPoint pos)
{
    volumeMenu->setFixedHeight(volumeMenu->sizeHint().height());
    volumeMenu->exec(QCursor::pos() - pos - QPoint(0, volumeMenu->height()/2)
                          + QPoint(width(), height() /2));
}

void SoundWidget::setMuted(bool mute)
{
    b_is_muted = mute;
    emit muteChanged(mute);
    //playlist_t *p_playlist = pl_Get(p_intf);
    //playlist_MuteSet(p_playlist, mute);
}

bool SoundWidget::eventFilter(QObject *obj, QEvent *e)
{
    Q_UNUSED(obj);
    if(e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *event = static_cast<QMouseEvent*>(e);
        if(event->button() == Qt::LeftButton)
        {
            if(volumeSlider->orientation() ==  Qt::Vertical)
            {
                showVolumeMenu(event->pos());
            }
            else
            {
                setMuted(!b_is_muted);
            }
            e->accept();
            return true;
        }
    }
    e->ignore();
    return false;
}
