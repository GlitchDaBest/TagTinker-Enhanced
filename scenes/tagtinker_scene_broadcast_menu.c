/*

 * Broadcast menu - page select / diagnostic page / party modes

 */



#include "../tagtinker_app.h"



static void broadcast_menu_cb(void* ctx, uint32_t index) {

    TagTinkerApp* app = ctx;

    view_dispatcher_send_custom_event(app->view_dispatcher, index);

}



void tagtinker_scene_broadcast_menu_on_enter(void* ctx) {

    TagTinkerApp* app = ctx;



    submenu_reset(app->submenu);

    submenu_set_header(app->submenu, "Broadcast Payloads");



    submenu_add_item(app->submenu, "Change Page", TagTinkerBroadcastFlipPage, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Page Party", TagTinkerBroadcastPageParty, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Diagnostic Page", TagTinkerBroadcastDebugScreen, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "LED Party", TagTinkerBroadcastLed, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "LED Sweep", TagTinkerBroadcastLedSweep, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Rave", TagTinkerBroadcastRave, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Chaos Timer", TagTinkerBroadcastChaos, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Text: SOLDES", TagTinkerBroadcastQuickTextSoldes, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Text: PROMO", TagTinkerBroadcastQuickTextPromo, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Text: SALE", TagTinkerBroadcastQuickTextSale, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Image fav1.bmp", TagTinkerBroadcastImageFav1, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Image fav2.bmp", TagTinkerBroadcastImageFav2, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Image fav3.bmp", TagTinkerBroadcastImageFav3, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Image EXP", TagTinkerBroadcastImage, broadcast_menu_cb, app);

    submenu_add_item(app->submenu, "Text EXP", TagTinkerBroadcastText, broadcast_menu_cb, app);
    submenu_add_item(app->submenu, "? Help / Guide", TagTinkerBroadcastHelp, broadcast_menu_cb, app);

    submenu_set_selected_item(

        app->submenu,

        scene_manager_get_scene_state(app->scene_manager, TagTinkerSceneBroadcastMenu));



    view_dispatcher_switch_to_view(app->view_dispatcher, TagTinkerViewSubmenu);

}



static bool broadcast_menu_go_broadcast(TagTinkerApp* app, uint8_t type) {

    app->broadcast_type = type;

    scene_manager_next_scene(app->scene_manager, TagTinkerSceneBroadcast);

    return true;

}



bool tagtinker_scene_broadcast_menu_on_event(void* ctx, SceneManagerEvent event) {

    TagTinkerApp* app = ctx;

    if(event.type != SceneManagerEventTypeCustom) return false;



    scene_manager_set_scene_state(app->scene_manager, TagTinkerSceneBroadcastMenu, event.event);



    switch(event.event) {

    case TagTinkerBroadcastFlipPage:

    case TagTinkerBroadcastPageParty:

    case TagTinkerBroadcastDebugScreen:

    case TagTinkerBroadcastLed:

    case TagTinkerBroadcastLedSweep:

    case TagTinkerBroadcastRave:

    case TagTinkerBroadcastChaos:

        return broadcast_menu_go_broadcast(app, (uint8_t)event.event);

    case TagTinkerBroadcastImage:

        app->broadcast_type = TagTinkerBroadcastImage;

        scene_manager_next_scene(app->scene_manager, TagTinkerSceneSyncedImageList);

        return true;

    case TagTinkerBroadcastText:

        app->broadcast_type = TagTinkerBroadcastText;

        memset(app->text_input_buf, 0, sizeof(app->text_input_buf));

        scene_manager_set_scene_state(

            app->scene_manager, TagTinkerSceneTextInput, TagTinkerTextInputNewText);

        scene_manager_next_scene(app->scene_manager, TagTinkerSceneTextInput);

        return true;

    case TagTinkerBroadcastQuickTextSoldes:

        tagtinker_start_broadcast_quick_text(app, "SOLDES");

        scene_manager_next_scene(app->scene_manager, TagTinkerSceneSizePicker);

        return true;

    case TagTinkerBroadcastQuickTextPromo:

        tagtinker_start_broadcast_quick_text(app, "PROMO");

        scene_manager_next_scene(app->scene_manager, TagTinkerSceneSizePicker);

        return true;

    case TagTinkerBroadcastQuickTextSale:

        tagtinker_start_broadcast_quick_text(app, "SALE");

        scene_manager_next_scene(app->scene_manager, TagTinkerSceneSizePicker);

        return true;

    case TagTinkerBroadcastHelp:
        tagtinker_show_text_box_help(app, TagTinkerTextBoxHelpBroadcast);
        return true;
    case TagTinkerBroadcastImageFav1:
    case TagTinkerBroadcastImageFav2:
    case TagTinkerBroadcastImageFav3: {
        const uint8_t slot = (uint8_t)(event.event - TagTinkerBroadcastImageFav1 + 1U);
        if(tagtinker_try_broadcast_fav_bmp(app, slot)) {
            scene_manager_next_scene(app->scene_manager, TagTinkerSceneTransmit);
            return true;
        }
        notification_message(app->notifications, &sequence_error);
        return true;
    }

    }

    return false;

}



void tagtinker_scene_broadcast_menu_on_exit(void* ctx) {

    TagTinkerApp* app = ctx;

    submenu_reset(app->submenu);

}

