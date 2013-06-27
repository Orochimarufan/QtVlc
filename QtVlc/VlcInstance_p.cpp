#include "VlcInstance_p.h"

#include <QStringList>

#include <QtVlc/enum.h>

VLC_WRAPPER_IMPL_CPP(VlcInstancePrivate, libvlc_instance_t, libvlc)

VlcInstancePrivate::VlcInstancePrivate(const QStringList &args) :
    VlcWrapperImpl(), d(nullptr)
{

    qRegisterMetaType<VlcState::Type>("VlcState::Type");
    qRegisterMetaType<VlcMeta::Type>("VlcMeta::Type");

    char **argv = (char **)malloc(sizeof(char **) * args.count());
    for (int i = 0; i < args.count(); i++)
        argv[i] = (char *)qstrdup(args.at(i).toLocal8Bit().data());

    d = libvlc_new(args.count(), argv);

    if (d)
        qDebug("Initialized libvlc.");
    else
        abort();

    VLC_WRAPPER_IMPL_INIT()
}

void VlcInstancePrivate::setUserAgent(const QString &appString, const QString &httpString)
{
    libvlc_set_user_agent(d, appString.toLocal8Bit().data(), httpString.toLocal8Bit().data());
}
