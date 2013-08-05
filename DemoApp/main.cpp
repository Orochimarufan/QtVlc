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

#include <QtWidgets/QApplication>
#include "VlcInstance.h"

#include "mainwindow.h"

#include <iostream>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    std::cout << "QtVlc " << qPrintable(VlcInstance::QtVlc_version()) \
              << " (" << qPrintable(VlcInstance::QtVlc_version_git()) << ")" << std::endl;
    std::cout << "built with libvlc-" << qPrintable(VlcInstance::QtVlc_build_libvlc_version()) \
              << " Qt-" << qPrintable(VlcInstance::QtVlc_build_qt_version()) << std::endl;
    std::cout << "libvlc " << qPrintable(VlcInstance::libvlc_version()) << std::endl;
    MainWindow win;
    win.show();
    return app.exec();
}
