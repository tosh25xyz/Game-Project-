#include "charselect.h"
#include "raygui.h"
#include <stddef.h>

/* --------------------------------------------------------------
   Edit this roster to match your actual assets. spritePath should
   point at a preview image for each character (a single frame is
   fine — doesn't need to be animated here).
   -------------------------------------------------------------- */
static const CharacterOption roster[] = {
    { "GOJO SATORU",     "Assets&resources/gojo.png", "The Strongest" ,"Assets&resources/gojo_matched_size.png"},
    { "SUKUNA",          "Assets&resources/sukuna.png",    "King of Curses",  "Assets&resources/king.png" },
    { "MEGUMI FUSHIGURO","Assets&resources/megumi.png",    "Ten Shadows User","Assets&resources/megumi2.png" },
    { "ITADORI YUJI", "Assets&resources/itadori.png",    "RAW STRENGTH","Assets&resources/yuji3.png" },
};
#define ROSTER_COUNT (int)(sizeof(roster) / sizeof(roster[0]))

static Texture2D previewTextures[ROSTER_COUNT];
static bool assetsLoaded = false;

int GetCharacterCount(void) { return ROSTER_COUNT; }

const CharacterOption *GetCharacterOption(int index)
{
    if (index < 0 || index >= ROSTER_COUNT) return NULL;
    return &roster[index];
}

void InitCharSelect(CharSelectState *cs)
{
    cs->selectedIndex = 0;
    cs->confirmed = false;
    cs->backPressed = false;
}

void LoadCharSelectAssets(void)
{
    for (int i = 0; i < ROSTER_COUNT; i++)
    {
        previewTextures[i] = LoadTexture(roster[i].spritePath);
        /* LoadTexture logs a warning and returns a zero-id texture on
           failure rather than crashing — DrawCharSelectPortrait below
           falls back to a colored placeholder box when that happens,
           so a missing preview image never breaks the screen. */
    }
    assetsLoaded = true;
}

void UnloadCharSelectAssets(void)
{
    if (!assetsLoaded) return;
    for (int i = 0; i < ROSTER_COUNT; i++)
    {
        if (previewTextures[i].id != 0) UnloadTexture(previewTextures[i]);
    }
    assetsLoaded = false;
}

static void DrawCharSelectBackdrop(int w, int h)
{
    DrawRectangleGradientV(0, 0, w, h,
        (Color){ 18, 10, 20, 255 },
        (Color){ 60, 20, 40, 255 });
}

static void DrawCharSelectPortrait(int index, Rectangle box)
{
    Texture2D tex = previewTextures[index];

    if (tex.id != 0)
    {
        float scale = box.height / (float)tex.height;
        float drawW = tex.width * scale;
        Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
        Rectangle dst = { box.x + (box.width - drawW) / 2.0f, box.y, drawW, box.height };
        DrawTexturePro(tex, src, dst, (Vector2){ 0, 0 }, 0.0f, WHITE);
    }
    else
    {
        /* fallback placeholder so a missing asset doesn't leave a
           blank hole in the layout */
        DrawRectangleRec(box, (Color){ 50, 50, 60, 255 });
        DrawRectangleLinesEx(box, 2, (Color){ 150, 150, 160, 255 });
        const char *label = "No Preview";
        int tw = MeasureText(label, 18);
        DrawText(label, (int)(box.x + box.width / 2 - tw / 2),
                 (int)(box.y + box.height / 2 - 9), 18, GRAY);
    }
}

/* Call every frame while in MODE_CHARACTER_SELECT. */
void UpdateDrawCharSelect(CharSelectState *cs, int screenW, int screenH)
{
    cs->confirmed = false;
    cs->backPressed = false;

    /* keyboard support alongside the on-screen buttons */
    if (IsKeyPressed(KEY_RIGHT)) cs->selectedIndex = (cs->selectedIndex + 1) % ROSTER_COUNT;
    if (IsKeyPressed(KEY_LEFT))  cs->selectedIndex = (cs->selectedIndex - 1 + ROSTER_COUNT) % ROSTER_COUNT;
    if (IsKeyPressed(KEY_ENTER)) cs->confirmed = true;

    DrawCharSelectBackdrop(screenW, screenH);

    const char *title = "CHOOSE YOUR SORCERER";
    int titleSize = 34;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, screenW / 2 - titleW / 2, 200, titleSize, WHITE);

    /* Portrait box, centered */
    Rectangle portraitBox = { screenW / 2.0f - 100, 350, 200, 220 };
    DrawCharSelectPortrait(cs->selectedIndex, portraitBox);

    const CharacterOption *opt = &roster[cs->selectedIndex];

    int nameSize = 26;
    int nameW = MeasureText(opt->name, nameSize);
    DrawText(opt->name, screenW / 2 - nameW / 2, (int)(portraitBox.y + portraitBox.height + 16), nameSize, WHITE);

    int tagSize = 16;
    int tagW = MeasureText(opt->tagline, tagSize);
    DrawText(opt->tagline, screenW / 2 - tagW / 2, (int)(portraitBox.y + portraitBox.height + 48),
              tagSize, (Color){ 220, 190, 200, 255 });

    /* Left / right carousel buttons flanking the portrait */
    Rectangle leftBtn  = { portraitBox.x - 60, portraitBox.y + portraitBox.height / 2 - 20, 44, 40 };
    Rectangle rightBtn = { portraitBox.x + portraitBox.width + 16, portraitBox.y + portraitBox.height / 2 - 20, 44, 40 };

    if (GuiButton(leftBtn, "<"))  cs->selectedIndex = (cs->selectedIndex - 1 + ROSTER_COUNT) % ROSTER_COUNT;
    if (GuiButton(rightBtn, ">")) cs->selectedIndex = (cs->selectedIndex + 1) % ROSTER_COUNT;

    /* Dot indicators showing roster position */
    float dotsWidth = ROSTER_COUNT * 18.0f;
    float dotsStartX = screenW / 2.0f - dotsWidth / 2.0f;
    for (int i = 0; i < ROSTER_COUNT; i++)
    {
        Color dotColor = (i == cs->selectedIndex) ? WHITE : (Color){ 120, 100, 110, 255 };
        DrawCircle((int)(dotsStartX + i * 18 + 6), (int)(portraitBox.y + portraitBox.height + 78), 5, dotColor);
    }

    /* Confirm / Back buttons */
    Rectangle confirmBtn = { screenW / 2.0f - 150, screenH - 90.0f, 300, 46 };
    Rectangle backBtn    = { 20, screenH - 60.0f, 120, 36 };

    if (GuiButton(confirmBtn, "Select")) cs->confirmed = true;
    if (GuiButton(backBtn, "< Back"))    cs->backPressed = true;
}