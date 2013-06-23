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
 * input_slider.cpp : VolumeSlider and SeekSlider
 ****************************************************************************
 * Copyright (C) 2006-2011 the VideoLAN team
 * $Id: 3a92a7809d299a336ad3fa35bacec95870a99871 $
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *          Ludovic Fauvet <etix@videolan.org>
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

#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>

#include <QtVlcWidgets/SoundSlider.h>

/* This work is derived from Amarok's work under GPLv2+
    - Mark Kretschmann
    - Gábor Lehel
   */

SoundSlider::SoundSlider( QWidget *_parent, float _i_step,
                          QString psz_colors, int max )
                        : QAbstractSlider( _parent )
{
    f_step = (float)(_i_step * 10000)
           / /*(float)((max - SOUNDMIN) * AOUT_VOLUME_DEFAULT)*/ SOUNDMAX;
    setRange( SOUNDMIN, max);
    setMouseTracking( true );
    isSliding = false;
    b_mouseOutside = true;
    b_isMuted = false;

    pixOutside = QPixmap( ":/toolbar/volslide-outside" );

    const QPixmap temp( ":/toolbar/volslide-inside" );
    const QBitmap mask( temp.createHeuristicMask() );

    setFixedSize( pixOutside.size() );

    pixGradient = QPixmap( mask.size() );
    pixGradient2 = QPixmap( mask.size() );

    /* Gradient building from the preferences */
    QLinearGradient gradient( paddingL, 2, WLENGTH + paddingL , 2 );
    QLinearGradient gradient2( paddingL, 2, WLENGTH + paddingL , 2 );

    QStringList colorList = psz_colors.split( ";" );

    /* Fill with 255 if the list is too short */
    if( colorList.count() < 12 )
        for( int i = colorList.count(); i < 12; i++)
            colorList.append( "255" );

    background = palette().color( QPalette::Active, QPalette::Window );
    foreground = palette().color( QPalette::Active, QPalette::WindowText );
    foreground.setHsv( foreground.hue(),
                    ( background.saturation() + foreground.saturation() ) / 2,
                    ( background.value() + foreground.value() ) / 2 );

    textfont.setPixelSize( 9 );
    textrect.setRect( 0, 0, 34, 15 );

    /* Regular colors */
#define c(i) colorList.at(i).toInt()
#define add_color(gradient, range, c1, c2, c3) \
    gradient.setColorAt( range, QColor( c(c1), c(c2), c(c3) ) );

    /* Desaturated colors */
#define desaturate(c) c->setHsvF( c->hueF(), 0.2 , 0.5, 1.0 )
#define add_desaturated_color(gradient, range, c1, c2, c3) \
    foo = new QColor( c(c1), c(c2), c(c3) );\
    desaturate( foo ); gradient.setColorAt( range, *foo );\
    delete foo;

    /* combine the two helpers */
#define add_colors( gradient1, gradient2, range, c1, c2, c3 )\
    add_color( gradient1, range, c1, c2, c3 ); \
    add_desaturated_color( gradient2, range, c1, c2, c3 );

    float f_mid_point = ( 100.0 / maximum() );
    QColor * foo;
    add_colors( gradient, gradient2, 0.0, 0, 1, 2 );
    add_colors( gradient, gradient2, f_mid_point - 0.05, 3, 4, 5 );
    add_colors( gradient, gradient2, f_mid_point + 0.05, 6, 7, 8 );
    add_colors( gradient, gradient2, 1.0, 9, 10, 11 );

    painter.begin( &pixGradient );
    painter.setPen( Qt::NoPen );
    painter.setBrush( gradient );
    painter.drawRect( pixGradient.rect() );
    painter.end();

    painter.begin( &pixGradient2 );
    painter.setPen( Qt::NoPen );
    painter.setBrush( gradient2 );
    painter.drawRect( pixGradient2.rect() );
    painter.end();

    pixGradient.setMask( mask );
    pixGradient2.setMask( mask );
}

void SoundSlider::wheelEvent( QWheelEvent *event )
{
    int newvalue = value() + event->delta() / ( 8 * 15 ) * f_step;
    setValue( qMin( qMax( minimum(), newvalue ), maximum() ) );

    emit sliderReleased();
    emit sliderMoved( value() );
}

void SoundSlider::mousePressEvent( QMouseEvent *event )
{
    if( event->button() != Qt::RightButton )
    {
        /* We enter the sliding mode */
        isSliding = true;
        i_oldvalue = value();
        emit sliderPressed();
        changeValue( event->x() - paddingL );
        emit sliderMoved( value() );
    }
}

void SoundSlider::processReleasedButton()
{
    if( !b_mouseOutside && value() != i_oldvalue )
    {
        emit sliderReleased();
        setValue( value() );
        emit sliderMoved( value() );
    }
    isSliding = false;
    b_mouseOutside = false;
}

void SoundSlider::mouseReleaseEvent( QMouseEvent *event )
{
    if( event->button() != Qt::RightButton )
        processReleasedButton();
}

void SoundSlider::mouseMoveEvent( QMouseEvent *event )
{
    /* handle mouserelease hijacking */
    if ( isSliding && ( event->buttons() & ~Qt::RightButton ) == Qt::NoButton )
        processReleasedButton();

    if( isSliding )
    {
        QRect rect( paddingL - 15,    -1,
                    WLENGTH + 15 * 2 , WHEIGHT + 5 );
        if( !rect.contains( event->pos() ) )
        { /* We are outside */
            if ( !b_mouseOutside )
                setValue( i_oldvalue );
            b_mouseOutside = true;
        }
        else
        { /* We are inside */
            b_mouseOutside = false;
            changeValue( event->x() - paddingL );
            emit sliderMoved( value() );
        }
    }
    else
    {
        int i = ( ( event->x() - paddingL ) * maximum() + 40 ) / WLENGTH;
        i = qMin( qMax( 0, i ), maximum() );
        setToolTip( QString("%1  %" ).arg( i ) );
    }
}

void SoundSlider::changeValue( int x )
{
    setValue( (x * maximum() + 40 ) / WLENGTH );
}

void SoundSlider::setMuted( bool m )
{
    b_isMuted = m;
    update();
}

void SoundSlider::paintEvent( QPaintEvent *e )
{
    QPixmap *paintGradient;
    if (b_isMuted)
        paintGradient = &this->pixGradient2;
    else
        paintGradient = &this->pixGradient;

    painter.begin( this );

    const int offset = int( ( WLENGTH * value() + 100 ) / maximum() ) + paddingL;

    const QRectF boundsG( 0, 0, offset , paintGradient->height() );
    painter.drawPixmap( boundsG, *paintGradient, boundsG );

    const QRectF boundsO( 0, 0, pixOutside.width(), pixOutside.height() );
    painter.drawPixmap( boundsO, pixOutside, boundsO );

    painter.setPen( foreground );
    painter.setFont( textfont );
    painter.drawText( textrect, Qt::AlignRight | Qt::AlignVCenter,
                      QString::number( value() ) + '%' );

    painter.end();
    e->accept();
}
