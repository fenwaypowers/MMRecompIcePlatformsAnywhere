#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "attributes.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"


void queue_icefloe(PlayState* play) {
    ObjectContext* objectCtx = &play->objectCtx;

    if (Object_GetSlot(objectCtx, OBJECT_ICEFLOE) == OBJECT_SLOT_NONE) {

        if (objectCtx->numEntries >= ARRAY_COUNT(objectCtx->slots)) {
            recomp_printf("No free object slots for OBJECT_ICEFLOE\n");
            return;
        }

        s32 slot = objectCtx->numEntries;

        void* nextPtr = func_8012F73C(objectCtx, slot, OBJECT_ICEFLOE);

        if (slot < ARRAY_COUNT(objectCtx->slots) - 1) {
            objectCtx->slots[slot + 1].segment = nextPtr;
        }

        objectCtx->numEntries++;

        recomp_printf("Queued OBJECT_ICEFLOE in slot %d\n", slot);

        return;
    }
}


RECOMP_HOOK("func_8088AA98") void before_func_8088AA98(EnArrow* this, PlayState* play) {
    if (recomp_get_config_u32("allow_anywhere") == 0) {
        queue_icefloe(play);
    }
}
