#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "attributes.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"

RECOMP_HOOK("func_8088AA98") void before_func_8088AA98(EnArrow* this, PlayState* play) {
    u32 enum_option = recomp_get_config_u32("enum_option");
    recomp_printf("enum_option: %d\n", enum_option);

    ObjectContext* objectCtx = &play->objectCtx;

    if (Object_GetSlot(objectCtx, OBJECT_ICEFLOE) == OBJECT_SLOT_NONE) {

        s32 slot = objectCtx->numEntries;

        func_8012F73C(objectCtx, slot, OBJECT_ICEFLOE);

        objectCtx->numEntries++;

        recomp_printf("Queued OBJECT_ICEFLOE in slot %d\n", slot);

        return;
    }
}
