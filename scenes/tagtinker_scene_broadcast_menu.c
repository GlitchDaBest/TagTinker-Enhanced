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
        app->page = 0; /* default restore away from Diagnostic */
        return broadcast_menu_go_broadcast(app, (uint8_t)event.event);

    case TagTinkerBroadcastPageParty:

    case TagTinkerBroadcastDebugScreen:

    case TagTinkerBroadcastLed:

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

    case TagTinkerBroadcastHelp:
        tagtinker_show_text_box_help(app, TagTinkerTextBoxHelpBroadcast);
        return true;

    }

    return false;

}



void tagtinker_scene_broadcast_menu_on_exit(void* ctx) {

    TagTinkerApp* app = ctx;

    submenu_reset(app->submenu);

}

