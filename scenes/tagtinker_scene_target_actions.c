/*
 * Target actions scene.
 */

#include "../tagtinker_app.h"

static void target_actions_cb(void* ctx, uint32_t index) {
    TagTinkerApp* app = ctx;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

static bool confirm_target_action(TagTinkerApp* app, const char* header, const char* body, const char* action) {
    if(!app || !header || !body || !action) return false;

    DialogMessage* message = dialog_message_alloc();
    dialog_message_set_header(message, header, 64, 2, AlignCenter, AlignTop);
    dialog_message_set_text(message, body, 64, 18, AlignCenter, AlignTop);
    dialog_message_set_buttons(message, "Back", NULL, action);
    DialogMessageButton button = dialog_message_show(app->dialogs, message);
    dialog_message_free(message);
    return button == DialogMessageButtonRight;
}

static void show_target_details(TagTinkerApp* app, const TagTinkerTarget* target) {
    if(!target) return;

    text_box_reset(app->text_box);
    text_box_set_font(app->text_box, TextBoxFontText);
    text_box_set_focus(app->text_box, TextBoxFocusStart);

    static char details_buf[640];
    snprintf(
        details_buf,
        sizeof(details_buf),
        "--- Tag Info ---\n"
        "Model: %s\n"
        "Type: %u (%s)\n"
        "Size: %ux%u\n"
        "Color: %s\n"
        "Barcode:\n%s\n"
        "\n"
        "--- Image upload ---\n"
        "If IR works but no red confirm\n"
        "LED + no new image: the tag did\n"
        "not accept the bitmap (wrong\n"
        "Type vs panel, or firmware).\n"
        "Common tags:\n"
        "1339 F62: LED classic (party).\n"
        "152x152 red image OK.\n"
        "1314 HD110: 400x300 mono.\n"
        "Use Image Prep type 1314.\n"
        "1605/1503 = Color M 208x112 red.\n"
        "1358 SmartTAG Color HD200 7.3\" 800x480.\n"
        "1318/1327 = HD M 208x112.\n"
        "1695 = 296x152 EXP only.\n"
        "Image Prep: pick matching Type.\n"
        "USB log: BMP TX line on send.\n"
        "LED: flash only (no RGB yet).\n"
        "LED Party / Flash / Locator.",
        target->profile.model_name ? target->profile.model_name : "Unknown",
        target->profile.type_code,
        tagtinker_profile_kind_label(target->profile.kind),
        target->profile.width,
        target->profile.height,
        tagtinker_profile_color_label(target->profile.color),
        target->barcode);

    text_box_set_text(app->text_box, details_buf);

    scene_manager_next_scene(app->scene_manager, TagTinkerSceneTextBox);
}

void tagtinker_scene_target_actions_on_enter(void* ctx) {
    TagTinkerApp* app = ctx;
    TagTinkerTarget* target = (app->selected_target >= 0) ? &app->targets[app->selected_target] : NULL;
    bool allow_graphics = tagtinker_target_supports_graphics(target);

    submenu_reset(app->submenu);

    char header[24];
    snprintf(
        header,
        sizeof(header),
        "%s",
        (target && target->name[0]) ? target->name : "Target");
    submenu_set_header(app->submenu, header);
    submenu_add_item(app->submenu, "Show Tag Info", TagTinkerTargetDetails, target_actions_cb, app);
    submenu_add_item(app->submenu, "Rename Tag", TagTinkerTargetRename, target_actions_cb, app);

    if(allow_graphics) {
        submenu_add_item(app->submenu, "Set Text", TagTinkerTargetPushText, target_actions_cb, app);
        submenu_add_item(app->submenu, "Set Image", TagTinkerTargetPushSyncedImage, target_actions_cb, app);
        submenu_add_item(app->submenu, "WiFi Plugins", TagTinkerTargetWifiPlugins, target_actions_cb, app);
    }

    submenu_add_item(app->submenu, "LED Flash", TagTinkerTargetPingFlash, target_actions_cb, app);
    submenu_add_item(app->submenu, "LED Locator", TagTinkerTargetLedLocator, target_actions_cb, app);
    submenu_add_item(app->submenu, "Delete Tag", TagTinkerTargetDeleteTag, target_actions_cb, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, TagTinkerViewSubmenu);
}

bool tagtinker_scene_target_actions_on_event(void* ctx, SceneManagerEvent event) {
    TagTinkerApp* app = ctx;
    if(event.type != SceneManagerEventTypeCustom) return false;

    switch(event.event) {
    case TagTinkerTargetDetails:
        show_target_details(app, &app->targets[app->selected_target]);
        return true;
    case TagTinkerTargetRename:
        scene_manager_set_scene_state(
            app->scene_manager, TagTinkerSceneTextInput, TagTinkerTextInputRenameTarget);
        scene_manager_next_scene(app->scene_manager, TagTinkerSceneTextInput);
        return true;
    case TagTinkerTargetPushText:
        if(!tagtinker_target_supports_graphics(&app->targets[app->selected_target])) return true;
        app->broadcast_type = TagTinkerBroadcastFlipPage;
        scene_manager_next_scene(app->scene_manager, TagTinkerScenePresetList);
        return true;
    case TagTinkerTargetPushSyncedImage:
        if(!tagtinker_target_supports_graphics(&app->targets[app->selected_target])) return true;
        app->broadcast_type = TagTinkerBroadcastFlipPage;
        scene_manager_next_scene(app->scene_manager, TagTinkerSceneSyncedImageList);
        return true;
    case TagTinkerTargetWifiPlugins:
        if(!tagtinker_target_supports_graphics(&app->targets[app->selected_target])) return true;
        app->broadcast_type = TagTinkerBroadcastFlipPage;
        scene_manager_next_scene(app->scene_manager, TagTinkerSceneWifiPlugins);
        return true;
    case TagTinkerTargetPingFlash:
    case TagTinkerTargetPingFlashClassic:
        {
            TagTinkerTarget* target = &app->targets[app->selected_target];
            const bool classic_first = tagtinker_led_subcmd_use_classic_map(
                target->profile.type_code);

            app->frame_seq_count = 4;
            app->frame_sequence = malloc(sizeof(uint8_t*) * 4);
            app->frame_lengths  = malloc(sizeof(size_t) * 4);
            app->frame_repeats  = malloc(sizeof(uint16_t) * 4);

            for(size_t i = 0; i < 4; i++) {
                app->frame_sequence[i] = malloc(TAGTINKER_MAX_FRAME_SIZE);
            }

            uint8_t blink_payload[6];
            size_t idx = 0;
            tagtinker_fill_led_blink_subcmd_map(
                blink_payload, TAGTINKER_LED_FLASH_DURATION_CLASSIC_SEC, false, 2U, true);
            app->frame_lengths[idx] =
                tagtinker_make_ping_frame(app->frame_sequence[idx], target->plid);
            app->frame_repeats[idx] = 160;
            idx++;
            app->frame_lengths[idx] = tagtinker_make_addressed_frame(
                app->frame_sequence[idx], target->plid, blink_payload, 6);
            app->frame_repeats[idx] = 80;
            idx++;
            if(!classic_first) {
                tagtinker_fill_led_blink_subcmd_map(
                    blink_payload, TAGTINKER_LED_FLASH_DURATION_SEC, false, 2U, false);
                app->frame_lengths[idx] =
                    tagtinker_make_ping_frame(app->frame_sequence[idx], target->plid);
                app->frame_repeats[idx] = 160;
                idx++;
                app->frame_lengths[idx] = tagtinker_make_addressed_frame(
                    app->frame_sequence[idx], target->plid, blink_payload, 6);
                app->frame_repeats[idx] = 80;
                idx++;
            }
            app->frame_seq_count = idx;

            memcpy(app->frame_buf, app->frame_sequence[0], app->frame_lengths[0]);
            app->frame_len = app->frame_lengths[0];

            app->tx_spam = false;
            app->broadcast_type = TagTinkerBroadcastLed;
            scene_manager_next_scene(app->scene_manager, TagTinkerSceneTransmit);
        }
        return true;
    case TagTinkerTargetLedLocator:
        app->broadcast_type = TagTinkerTransmitLedLocator;
        app->tx_spam = false;
        tagtinker_free_frame_sequence(app);
        scene_manager_next_scene(app->scene_manager, TagTinkerSceneTransmit);
        return true;
    case TagTinkerTargetDeleteTag:
        {
            if(app->selected_target < 0 || app->selected_target >= app->target_count) {
                return true;
            }
            TagTinkerTarget* target = &app->targets[app->selected_target];
            char body[96];
            snprintf(body, sizeof(body), "Delete %s and its\nsaved images?", target->name);
            if(!confirm_target_action(app, "Delete Tag", body, "Delete")) {
                return true;
            }

            tagtinker_delete_target(app, (uint8_t)app->selected_target);
            scene_manager_search_and_switch_to_previous_scene(
                app->scene_manager, TagTinkerSceneTargetMenu);
        }
        return true;
    }
    return false;
}

void tagtinker_scene_target_actions_on_exit(void* ctx) {
    TagTinkerApp* app = ctx;
    submenu_reset(app->submenu);
}
