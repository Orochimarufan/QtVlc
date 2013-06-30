#include "VlcInstance_p.h"

// On other platforms, the Janitor gets called from atexit(), Qt will already be gone.
#ifndef __GNUC__
#define NO_LEAK_DETECTION
#define SODESTRUCT
#else
#define SODESTRUCT __attribute__((destructor))
#endif

#ifndef NO_LEAK_DETECTION
#include "VlcMedia_p.h"
#include "VlcMediaPlayer_p.h"
#include "VlcMediaList_p.h"
#include "VlcMediaListPlayer_p.h"
#endif

struct __Janitor__
{
    static void killGlobalInstance()
    {
        if (VlcInstancePrivate::global != nullptr)
            delete VlcInstancePrivate::global;
    }

#ifndef NO_LEAK_DETECTION
    static void detectLeaks()
    {
        detectLeaksOf<VlcMediaListPlayerPrivate>();
        detectLeaksOf<VlcMediaPlayerPrivate>();
        detectLeaksOf<VlcMediaListPrivate>();
        detectLeaksOf<VlcMediaPrivate>();
        detectLeaksOf<VlcInstancePrivate>();
    }

    template <typename T>
    static void detectLeaksOf()
    {
        if (!T::instances.isEmpty())
        {
            qWarning("Leaking %i %s instance(s)!", T::instances.count(), T::staticMetaObject.className());
            QList<T *> v = T::instances.values();
            QListIterator<T *> it(v);
            while (it.hasNext())
            {
                T *self = it.next();
                qWarning("Deleting %s instance with %i references.", T::staticMetaObject.className(), self->ref.load());
                delete self;
            }
        }
    }
#endif
};

SODESTRUCT
void __Janitor__run()
{
    __Janitor__::killGlobalInstance();

#ifndef NO_LEAK_DETECTION
    __Janitor__::detectLeaks();
#endif
}
