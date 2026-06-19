/*
 * In-app help texts.
 */

#include "tagtinker_app.h"

static const char* const help_broadcast =
    "--- Broadcast (all tags) ---\n"
    "\n"
    "Change Page: show page 0-7 on\n"
    "tags that listen to broadcast.\n"
    "\n"
    "Page Party: cycles pages 0-7.\n"
    "\n"
    "LED Party: flash all tags in\n"
    "range (classic + scan IR codes).\n"
    "No RGB colours — blink only.\n"
    "Use Repeat for continuous.\n"
    "\n"
    "LED Sweep: flash each saved\n"
    "target (needs NFC register).\n"
    "\n"
    "Chaos: LED party + pages +\n"
    "target sweep.\n"
    "\n"
    "Image / text: see menus.\n"
    "BMP names: WIDTHxHEIGHT_*.bmp\n";

static const char* const help_target =
    "--- Targeted (one tag) ---\n"
    "\n"
    "NFC: + Scan NFC (one tag) or\n"
    "+ Quick Scan NFC (mass add).\n"
    "Set Image with Image Prep\n"
    "(match Type).\n"
    "\n"
    "Common: 1339 F62 152x152,\n"
    "1314 HD110 400x300 mono,\n"
    "1605 Color M 208x112.\n"
    "1358 SmartTAG Color HD200 7.3\"\n"
    "BWR — Image Prep 1358 RED.\n"
    "\n"
    "LED Flash: one tag ping+blink.\n"
    "LED Locator: blink until [<-].\n"
    "\n"
    "RGB LED colours via IR not\n"
    "found yet — flash patterns only.\n";

void tagtinker_show_text_box_help(TagTinkerApp* app, TagTinkerTextBoxKind kind) {
    if(!app) return;

    const char* text = help_broadcast;
    if(kind == TagTinkerTextBoxHelpTarget) {
        text = help_target;
    }

    text_box_reset(app->text_box);
    text_box_set_font(app->text_box, TextBoxFontText);
    text_box_set_focus(app->text_box, TextBoxFocusStart);
    text_box_set_text(app->text_box, text);

    scene_manager_set_scene_state(app->scene_manager, TagTinkerSceneTextBox, (uint32_t)kind);
    scene_manager_next_scene(app->scene_manager, TagTinkerSceneTextBox);
}
