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
 * seekstyle.cpp : Seek slider style
 ****************************************************************************
 * Copyright (C) 2011-2012 VLC authors and VideoLAN
 *
 * Authors: Ludovic Fauvet <etix@videolan.org>
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

#include <QtWidgets/QStyleOptionSlider>
#include <QtGui/QPainter>
#include <QtCore/QDebug>

#include "SeekStyle.h"
#include "SeekSlider.h"

#define RADIUS 3
#define CHAPTERSSPOTSIZE 3

#define DEBUG_RECT(rect) qDebug("SeekStyle: rect '" #rect "': %s, %i/%i, %ix%i", rect.isValid()?"Valid":"Invalid", rect.x(), rect.y(), rect.width(), rect.height())

int SeekStyle::pixelMetric( PixelMetric metric, const QStyleOption *option, const QWidget *widget ) const
{
    const QStyleOptionSlider *slider;

    if ( metric == PM_SliderLength && ( slider = qstyleoption_cast<const QStyleOptionSlider *>( option ) ) )
        return slider->rect.height();
    else
        return QCommonStyle::pixelMetric( metric, option, widget );
}

void SeekStyle::drawComplexControl( ComplexControl cc, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget ) const
{
    if( cc == CC_Slider )
    {
        painter->setRenderHints( QPainter::Antialiasing );

        if ( const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>( option ) )
        {
            const SeekSlider *seekSlider = qobject_cast<const SeekSlider*>( widget );
            qreal sliderPos = -1;

            /* Get the needed subcontrols to draw the slider */
            QRect groove = subControlRect(CC_Slider, slider, SC_SliderGroove, widget);
            QRect handle = subControlRect(CC_Slider, slider, SC_SliderHandle, widget);

            // for some reason, the height gets set to 0
            if (groove.height() == 0)
            {
                groove.setHeight(widget->height());
                if (!printedWarning)
                {
                    qWarning() << "SeekStyle: got zero height!";
                    DEBUG_RECT(handle);
                    DEBUG_RECT(groove);
                    printedWarning = true;
                }
            }
            if (handle.height() == 0)
            {
                handle.setHeight(widget->height());
                if (!printedWarning)
                {
                    qWarning() << "SeekStyle: got zero height!";
                    DEBUG_RECT(handle);
                    DEBUG_RECT(groove);
                    printedWarning = true;
                }
            }

            //DEBUG_RECT(groove);
            //DEBUG_RECT(handle);

            /* Adjust the size of the groove so the handle stays centered */
            groove.adjust( handle.width() / 2, 0, -handle.width() / 2, 0 );

            /* Reduce the height of the groove */
            // Note: in the original 2.0.0 code, the groove had a height of 9px and to
            // comply with the original style (but still allow the widget to expand) I
            // had to remove 1 from the rect bottom.
            groove.adjust( 0, (qreal)groove.height() / 3.7, 0, (qreal)-groove.height() / 3.7 - 1 );

            if ( ( slider->subControls & SC_SliderGroove ) && groove.isValid() )
            {
                sliderPos = ( ( (qreal)groove.width() ) / (qreal)slider->maximum )
                        * (qreal)slider->sliderPosition;

                /* set the background color and gradient */
                QColor backgroundBase( slider->palette.window().color() );
                QLinearGradient backgroundGradient( 0, 0, 0, slider->rect.height() );
                backgroundGradient.setColorAt( 0.0, backgroundBase.darker( 140 ) );
                backgroundGradient.setColorAt( 1.0, backgroundBase );

                /* set the foreground color and gradient */
                QColor foregroundBase( 50, 156, 255 );
                QLinearGradient foregroundGradient( 0, 0, 0, groove.height() );
                foregroundGradient.setColorAt( 0.0,  foregroundBase );
                foregroundGradient.setColorAt( 1.0,  foregroundBase.darker( 125 ) );

                /* draw a slight 3d effect on the bottom */
                painter->setPen( QColor( 230, 230, 230 ) );
                painter->setBrush( Qt::NoBrush );
                painter->drawRoundedRect( groove.adjusted( 0, 2, 0, 0 ), RADIUS, RADIUS );

                /* draw background */
                painter->setPen( Qt::NoPen );
                painter->setBrush( backgroundGradient );
                painter->drawRoundedRect( groove, RADIUS, RADIUS );

                /* adjusted foreground rectangle */
                QRect valueRect = groove.adjusted( 1, 1, -1, 0 );

                valueRect.setWidth( sliderPos );

                /* draw foreground */
                if ( slider->sliderPosition > slider->minimum && slider->sliderPosition <= slider->maximum )
                {
                    painter->setPen( Qt::NoPen );
                    painter->setBrush( foregroundGradient );
                    painter->drawRoundedRect( valueRect, RADIUS, RADIUS );
                }

                /* draw buffering overlay */
                if ( seekSlider && seekSlider->f_buffering < 1.0 )
                {
                    QRect innerRect = groove.adjusted( 1, 1,
                                        groove.width() * ( -1.0 + seekSlider->f_buffering ) - 1, 0 );
                    QColor overlayColor = QColor( "Orange" );
                    overlayColor.setAlpha( 128 );
                    painter->setBrush( overlayColor );
                    painter->drawRoundedRect( innerRect, RADIUS, RADIUS );
                }
            }
            else qDebug("SeekStyle: no groove: %i %s, %ix%i at %i %i", (int)(slider->subControls & SC_SliderGroove), groove.isValid()?"Valid":"Invalid", groove.width(), groove.height(), groove.x(), groove.y());

            if ( slider->subControls & SC_SliderTickmarks ) {
                QStyleOptionSlider tmpSlider = *slider;
                tmpSlider.subControls = SC_SliderTickmarks;
                QCommonStyle::drawComplexControl(cc, &tmpSlider, painter, widget);
            }

            if ( slider->subControls & SC_SliderHandle && handle.isValid() )
            {
                /* Useful for debugging */
                //painter->setBrush( QColor( 0, 0, 255, 150 ) );
                //painter->drawRect( handle );

                if ( option->state & QStyle::State_MouseOver || (seekSlider && seekSlider->isAnimationRunning() ) )
                {
                    QPalette p = slider->palette;

#if 0
                    /* draw chapters tickpoints */
                    if ( seekSlider->chapters && seekSlider->inputLength && groove.width() )
                    {
                        QColor background = p.color( QPalette::Active, QPalette::Window );
                        QColor foreground = p.color( QPalette::Active, QPalette::WindowText );
                        foreground.setHsv( foreground.hue(),
                                        ( background.saturation() + foreground.saturation() ) / 2,
                                        ( background.value() + foreground.value() ) / 2 );
                        if ( slider->orientation == Qt::Horizontal ) /* TODO: vertical */
                        {
                            QList<SeekPoint> points = seekSlider->chapters->getPoints();
                            foreach( SeekPoint point, points )
                            {
                                int x = groove.x() + point.time / 1000000.0 / seekSlider->inputLength * groove.width();
                                painter->setPen( foreground );
                                painter->setBrush( Qt::NoBrush );
                                painter->drawLine( x, slider->rect.height(), x, slider->rect.height() - CHAPTERSSPOTSIZE );
                            }
                        }
                    }
#endif

                    /* draw handle */
                    if ( option->state & QStyle::State_Enabled && sliderPos != -1 )
                    {
                        QSize hSize = QSize( handle.height(), handle.height() ) - QSize( 6, 6 );;
                        QPoint pos = QPoint( handle.center().x() - ( hSize.width() / 2 ), handle.center().y() - ( hSize.height() / 2 ) );

                        QPoint shadowPos( pos - QPoint( 2, 2 ) );
                        QSize sSize( QSize( handle.height(), handle.height() ) - QSize( 2, 2 ) );

                        /* prepare the handle's gradient */
                        QLinearGradient handleGradient( 0, 0, 0, hSize.height() );
                        handleGradient.setColorAt( 0.0, p.window().color().lighter( 120 ) );
                        handleGradient.setColorAt( 0.9, p.window().color().darker( 120 ) );

                        /* prepare the handle's shadow gradient */
                        QColor shadowBase = p.shadow().color();
                        if( shadowBase.lightness() > 100 )
                            shadowBase = QColor( 60, 60, 60 ); // Palette's shadow is too bright
                        QColor shadowDark( shadowBase.darker( 150 ) );
                        QColor shadowLight( shadowBase.lighter( 180 ) );
                        shadowLight.setAlpha( 50 );

                        QRadialGradient shadowGradient( shadowPos.x() + ( sSize.width() / 2 ),
                                                        shadowPos.y() + ( sSize.height() / 2 ),
                                                        qMax( sSize.width(), sSize.height() ) / 2 );
                        shadowGradient.setColorAt( 0.4, shadowDark );
                        shadowGradient.setColorAt( 1.0, shadowLight );

                        painter->setPen( Qt::NoPen );
                        if ( seekSlider != NULL )
                            painter->setOpacity( seekSlider->mHandleOpacity );

                        /* draw the handle's shadow */
                        painter->setBrush( shadowGradient );
                        painter->drawEllipse( shadowPos.x(), shadowPos.y() + 1, sSize.width(), sSize.height() );

                        /* finally draw the handle */
                        painter->setBrush( handleGradient );
                        painter->drawEllipse( pos.x(), pos.y(), hSize.width(), hSize.height() );
                    }
                }
            }
        }
    }
    else
    {
        qWarning() << "SeekStyle: Drawing an unmanaged control";
        QCommonStyle::drawComplexControl( cc, option, painter, widget );
    }
}
