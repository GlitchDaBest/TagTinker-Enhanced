/*
 * Multi-Targeted: mass-add tags (NFC / barcode), then send image to all
 * saved graphics tags that match the BMP WxH — addressed PLID only (no
 * broadcast). Fixes F05-class 1327 that ignore PLID-0 image DATA.
 */

#include "../tagtinker_app.h"

enum {
    MultiTargetQuickNfc = 1,
    MultiTargetAddId = 2,
    MultiTargetSendImage = 3,
    MultiTargetHelp = 4,
};

static uint8_t multi_target_graphics_count(const TagTinkerApp* app) {
    uint8_t n = 0U;
    for(uint8_t i = 0; i < app->target_count; i++) {
        if(tagtinker_target_supports_graphics(&app->targets[i])) n++;
    }
    return n;
}

static void multi_target_cb(void* ctx, uint32_t index) {
    TagTinkerApp* app = ctx;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void tagtinker_scene_multi_target_on_enter(void* ctx) {
    TagTinkerApp* app = ctx;
    uint8_t gfx = multi_target_graphics_count(app);

    submenu_reset(app->submenu);

    char header[28];
    snprintf(header, sizeof(header), "Multi-Target (%u)", (unsigned)gfx);
    submenu_set_header(app->submenu, header);

    submenu_add_item(
        app->submenu, "+ Quick Scan NFC", MultiTargetQuickNfc, multi_target_cb, app);
    submenu_add_item(app->submenu, "+ Add by ID", MultiTargetAddId, multi_target_cb, app);

    char send_label[32];
    snprintf(send_label, sizeof(send_label), "Send Image (%u tags)", (unsigned)gfx);
    submenu_add_item(app->submenu, send_label, MultiTargetSendImage, multi_target_cb, app);
    submenu_add_item(app->submenu, "? Help", MultiTargetHelp, multi_target_cb, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, TagTinkerViewSubmenu);
}

bool tagtinker_scene_multi_target_on_event(void* ctx, SceneManagerEvent event) {
    TagTinkerApp* app = ctx;
    if(event.type != SceneManagerEventTypeCustom) return false;

    switch(event.event) {
    case MultiTargetQuickNfc:
        app->nfc_rapid_added = 0U;
        scene_manager_set_scene_state(
            app->scene_manager, TagTinkerSceneNfcScan, TagTinkerNfcScanRapid);
        scene_manager_next_scene(app->scene_manager, TagTinkerSceneNfcScan);
        return true;

    case MultiTargetAddId:
        scene_manager_set_scene_state(
            app->scene_manager, TagTinkerSceneBarcodeInput, TagTinkerSceneMultiTarget);
        scene_manager_next_scene(app->scene_manager, TagTinkerSceneBarcodeInput);
        return true;

    case MultiTargetSendImage:
        if(multi_target_graphics_count(app) == 0U) {
            popup_reset(app->popup);
            popup_set_header(app->popup, "No tags", 64, 20, AlignCenter, AlignCenter);
            popup_set_text(
                app->popup,
                "Quick Scan NFC or\nAdd by ID first",
                64,
                36,
                AlignCenter,
                AlignCenter);
            popup_set_timeout(app->popup, 2000);
            popup_enable_timeout(app->popup);
            popup_set_callback(app->popup, NULL);
            view_dispatcher_switch_to_view(app->view_dispatcher, TagTinkerViewPopup);
            return true;
        }
        app->broadcast_type = TagTinkerTransmitMultiTarget;
        scene_manager_next_scene(app->scene_manager, TagTinkerSceneSyncedImageList);
        return true;

    case MultiTargetHelp:
        tagtinker_show_text_box_help(app, TagTinkerTextBoxHelpTarget);
        return true;

    default:
        break;
    }
    return false;
}

void tagtinker_scene_multi_target_on_exit(void* ctx) {
    TagTinkerApp* app = ctx;
    submenu_reset(app->submenu);
}
