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
        detectLeaksOf<libvlc_media_player_t, VlcMediaPlayerPrivate>(VlcMediaPlayerPrivate::instances);
        detectLeaksOf<libvlc_media_list_t, VlcMediaListPrivate>(VlcMediaListPrivate::instances);
        detectLeaksOf<libvlc_media_t, VlcMediaPrivate>(VlcMediaPrivate::instances);
        detectLeaksOf<libvlc_instance_t, VlcInstancePrivate>(VlcInstancePrivate::instances);
    }

    template <typename S, typename T>
    static void detectLeaksOf(QHash<S *, T *> map)
    {
        if (!map.isEmpty())
        {
            qWarning("Leaking %i %s instance(s)!", map.count(), T::staticMetaObject.className());
            auto v = map.values();
            QListIterator<T *> it(v);
            while (it.hasNext())
                delete it.next();
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
