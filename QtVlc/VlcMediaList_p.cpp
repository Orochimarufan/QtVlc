#include "VlcMediaList_p.h"

#include <QtVlc/VlcMedia.h>

VLC_WRAPPER_IMPL_CPP2(VlcMediaListPrivate, libvlc_media_list_t,
                      libvlc_media_list, attach_events, detach_events)

VlcMediaListPrivate::VlcMediaListPrivate(libvlc_instance_t *instance) :
    VlcWrapperImpl()
{
    d = libvlc_media_list_new(instance);

    VLC_WRAPPER_IMPL_INIT();

    attach_events();
}

inline void VlcMediaListPrivate::attach_events()
{
    p_eventmanager = libvlc_media_list_event_manager(d);

#define ATTACH(event_type) libvlc_event_attach(p_eventmanager, event_type, &VlcMediaListPrivate::event_cb, this)
    ATTACH(libvlc_MediaListItemAdded);
    ATTACH(libvlc_MediaListWillAddItem);
    ATTACH(libvlc_MediaListItemDeleted);
    ATTACH(libvlc_MediaListWillDeleteItem);
#undef ATTACH
}

inline void VlcMediaListPrivate::detach_events()
{
#define DETACH(event_type) libvlc_event_detach(p_eventmanager, event_type, &VlcMediaListPrivate::event_cb, this)
    DETACH(libvlc_MediaListItemAdded);
    DETACH(libvlc_MediaListWillAddItem);
    DETACH(libvlc_MediaListItemDeleted);
    DETACH(libvlc_MediaListWillDeleteItem);
#undef DETACH
}

void VlcMediaListPrivate::event_cb(const libvlc_event_t *e, void *o)
{
    VlcMediaListPrivate *p = static_cast<VlcMediaListPrivate *>(o);

    emit p->libvlcEvent(e);

    switch(e->type)
    {
    case libvlc_MediaListWillAddItem:
        emit p->willAddItem(VlcMedia(e->u.media_list_will_add_item.item), e->u.media_list_will_add_item.index);
        break;
    case libvlc_MediaListItemAdded:
        emit p->itemAdded(VlcMedia(e->u.media_list_item_added.item), e->u.media_list_item_added.index);
        break;
    case libvlc_MediaListWillDeleteItem:
        emit p->willDeleteItem(VlcMedia(e->u.media_list_will_delete_item.item), e->u.media_list_will_delete_item.index);
        break;
    case libvlc_MediaListItemDeleted:
        emit p->itemDeleted(VlcMedia(e->u.media_list_item_deleted.item), e->u.media_list_item_deleted.index);
        break;
    default:
        qDebug("VlcMediaList: unknown Event: %i (%s)", e->type, libvlc_event_type_name(e->type));
    }
}

void VlcMediaListPrivate::setMedia(libvlc_media_t *p_md)
{
    D libvlc_media_list_set_media(d, p_md);
}

libvlc_media_t *VlcMediaListPrivate::media()
{
    D return libvlc_media_list_media(d);
    else return nullptr;
}

int VlcMediaListPrivate::addMedia(libvlc_media_t *p_md)
{
    D return libvlc_media_list_add_media(d, p_md);
    else return -1;
}

int VlcMediaListPrivate::insertMedia(libvlc_media_t *p_md, int at)
{
    D return libvlc_media_list_insert_media(d, p_md, at);
    else return -1;
}

int VlcMediaListPrivate::removeIndex(int at)
{
    D return libvlc_media_list_remove_index(d, at);
    else return -1;
}

int VlcMediaListPrivate::count()
{
    D return libvlc_media_list_count(d);
    else return -1;
}

libvlc_media_t *VlcMediaListPrivate::itemAtIndex(int at)
{
    D return libvlc_media_list_item_at_index(d, at);
    else return nullptr;
}

int VlcMediaListPrivate::indexOfItem(libvlc_media_t *p_md)
{
    D return libvlc_media_list_index_of_item(d, p_md);
    else return -1;
}

int VlcMediaListPrivate::isReadOnly()
{
    D return libvlc_media_list_is_readonly(d);
    else return -1;
}

void VlcMediaListPrivate::lock()
{
    D libvlc_media_list_lock(d);
}

void VlcMediaListPrivate::unlock()
{
    D libvlc_media_list_unlock(d);
}
