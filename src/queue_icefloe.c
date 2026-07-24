#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "attributes.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"

// Queue OBJECT_ICEFLOE for asynchronous loading if the current room
// does not already include it. This allows BgIcefloe to be spawned
// anywhere without requiring every room's object list to be modified.
void queue_icefloe(PlayState* play) {
    ObjectContext* objectCtx = &play->objectCtx;

    // The object is already present (or already queued for loading).
    if (Object_GetSlot(objectCtx, OBJECT_ICEFLOE) != OBJECT_SLOT_NONE) {
        return;
    }

    // Avoid writing past the end of the fixed-size object table.
    if (objectCtx->numEntries >= ARRAY_COUNT(objectCtx->slots)) {
        recomp_printf("No free object slots for OBJECT_ICEFLOE\n");
        return;
    }

    s32 slot = objectCtx->numEntries;

    // Reserve a new object slot. The object will actually be loaded by
    // Object_UpdateEntries() on a later frame.
    void* nextPtr = func_8012F73C(objectCtx, slot, OBJECT_ICEFLOE);

    // Preserve the segment pointer for the next potential object, matching
    // the behavior of Scene_CommandObjectList().
    if (slot < ARRAY_COUNT(objectCtx->slots) - 1) {
        objectCtx->slots[slot + 1].segment = nextPtr;
    }

    // Increment the number of entries to make the queued object visible to the object loader.
    objectCtx->numEntries++;

    recomp_printf("Queued OBJECT_ICEFLOE in slot %d\n", slot);
}

// Ensure OBJECT_ICEFLOE has been queued before an ice arrow attempts
// to spawn a BgIcefloe actor. By the time the actor is needed, the
// object will typically have finished loading asynchronously.
RECOMP_HOOK("func_8088AA98") void before_func_8088AA98(EnArrow* this, PlayState* play) {
    if (recomp_get_config_u32("allow_anywhere") == 0) {
        queue_icefloe(play);
    }
}
