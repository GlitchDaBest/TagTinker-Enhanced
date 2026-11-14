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
    "Image EXP: PLID0 broadcast only.\n"
    "Settings → Image EXP fan-out On\n"
    "also pushes saved same-WxH tags.\n"
    "F05 1327 often needs Multi-\n"
    "Targeted instead.\n"
    "\n"
    "BMP names: WIDTHxHEIGHT_*.bmp\n";

static const char* const help_target =
    "--- Targeted (one tag) ---\n"
    "\n"
    "NFC: + Scan NFC (one tag) or\n"
    "+ Quick Scan NFC (mass add).\n"
    "Set Image with Image Prep\n"
    "(match Type).\n"
    "\n"
    "Multi-Targeted: Quick Scan NFC\n"
    "+ Add by ID, then Send Image\n"
    "to ALL saved tags matching the\n"
    "BMP WxH (addressed, no PLID0).\n"
    "Use this for F05 + F08 together.\n"
    "\n"
    "Common: 1339 F62 152x152,\n"
    "1314 HD110 mono, 1336 HD110 Red.\n"
    "Both 400x300 — pick Image Prep\n"
    "profile matching NFC type.\n"
    "1605/1503 Color M 208x112 (HD M).\n"
    "1358 SmartTAG Color HD200 7.3\"\n"
    "800x480 mono — Image Prep 1358.\n"
    "\n"
    "LED Test: one tag ping+blink.\n"
    "LED Locator: blink until [<-].\n"
    "Wakeup Burst: after battery swap\n"
    "or deep sleep. Dense ping trains\n"
    "+ classic/scan LED maps catch\n"
    "sparse IR windows. Intensity:\n"
    "Normal / Strong / Max. Optional\n"
    "page-0 restore at the end.\n"
    "Burst time 15-180s. Cancel [<-].\n"
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
