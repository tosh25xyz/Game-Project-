#include"menu.h"
#include<stdio.h>
#include<math.h>
//#include"gamemode.h"
//#include"../raygui.h"

#define RAYGUI_IMPLEMENTATION
 #include "raygui.h"

 
void InitMenu(MenuState *menu)
{
    menu->selectedDifficulty     = DIFFICULTY_NORMAL;
    menu->difficultyDropdownOpen = false;
    menu->startPressed = false;
    menu->exitPressed  = false;
    menu->titlePulse   = 0.0f;
}
 
float DifficultyEnemyHpMultiplier(Difficulty d)
{
    if(d==0)      return 0.75f;
    else if(d==1) return 1.00f;
    else if(d==2) return 1.50f;
    else          return 1.00f;
}
 
/* ===============================================================
   MP4 BACKGROUND — decoded live via an ffmpeg pipe.
   ---------------------------------------------------------------
   ffmpeg is launched as a subprocess that writes raw RGBA frames
   to stdout; we read one frame's worth of bytes per tick and
   push it into a raylib texture with UpdateTexture(). No PNGs are
   ever written to disk — this decodes the mp4 on the fly.
 
   ffmpeg.exe must be reachable (PATH, or give a full path below).
   "-stream_loop -1" makes ffmpeg loop the input file forever, so
   the pipe never actually ends — no need to relaunch it.
 
   MinGW: popen/pclose are provided as aliases for _popen/_pclose.
   If your setup complains they're undeclared, replace popen with
   _popen and pclose with _pclose below.
   =============================================================== */
 
static FILE *videoPipe = NULL;
static unsigned char *videoBuffer = NULL;
static Texture2D videoTexture;
static int videoW = 0, videoH = 0;
static float videoFrameDuration = 1.0f / 24.0f;
static float videoFrameTimer = 0.0f;
static bool videoLoaded = false;
 
/* width/height should match (or be smaller than) your window size
   for best quality without wasted decode cost. fps controls both
   the ffmpeg output rate and how often we swap the texture. */
void LoadMenuBackgroundVideo(const char *videoPath, int width, int height, float fps)
{
    videoW = width;
    videoH = height;
    videoFrameDuration = 1.0f / fps;
    videoFrameTimer = 0.0f;
 
    const char *cmd = TextFormat(
        "ffmpeg -stream_loop -1 -i \"%s\" -f rawvideo -pix_fmt rgba "
        "-vf scale=%d:%d -r %.2f -loglevel quiet -",
        videoPath, width, height, fps);
 
    videoPipe = popen(cmd, "rb");
    FILE *log = fopen("bgvideo_debug.txt", "w");
    if (log) {
        fprintf(log, "Command: %s\n", cmd);
        fprintf(log, "FileExists(videoPath): %d\n", FileExists(videoPath));
        fprintf(log, "popen result: %s\n", videoPipe ? "OK" : "NULL (failed)");
        fclose(log);
    }
    if (videoPipe == NULL)
    {
        TraceLog(LOG_WARNING, "Could not start ffmpeg for background video");
        videoLoaded = false;
        return;
    }
 
    videoBuffer = (unsigned char *)malloc((size_t)width * height * 4);
 
    Image blank = GenImageColor(width, height, BLACK);
    videoTexture = LoadTextureFromImage(blank);
    UnloadImage(blank);
 
    videoLoaded = true;
}
 
void UnloadMenuBackgroundVideo(void)
{
    if (videoPipe) { pclose(videoPipe); videoPipe = NULL; }
    if (videoBuffer) { free(videoBuffer); videoBuffer = NULL; }
    if (videoLoaded) { UnloadTexture(videoTexture); videoLoaded = false; }
}
 
/* Reads one frame from the pipe when it's time and uploads it to
   the GPU texture, then draws it stretched to fill the screen. */
static void DrawMenuBackdrop(int w, int h)
{
    if (!videoLoaded)
    {
        ClearBackground((Color){ 10, 16, 34, 255 });
        return;
    }
 
    videoFrameTimer += GetFrameTime();
    if (videoFrameTimer >= videoFrameDuration)
    {
        videoFrameTimer -= videoFrameDuration;
 
        size_t frameBytes = (size_t)videoW * videoH * 4;
        size_t readBytes = fread(videoBuffer, 1, frameBytes, videoPipe);
 
        if (readBytes == frameBytes)
        {
            UpdateTexture(videoTexture, videoBuffer);
        }
        /* if readBytes is short, the pipe hiccuped this tick —
           just keep showing the last uploaded frame, no crash */
    }
 
    Rectangle src = { 0, 0, (float)videoW, (float)videoH };
    Rectangle dst = { 0, 0, (float)w, (float)h };
    DrawTexturePro(videoTexture, src, dst, (Vector2){ 0, 0 }, 0.0f, WHITE);
}
 
/* Call every frame while in MODE_MAIN_MENU. */
void UpdateDrawMenu(MenuState *menu, int screenW, int screenH)
{
    menu->startPressed = false;
    menu->exitPressed  = false;
    menu->titlePulse  += GetFrameTime();
 
    DrawMenuBackdrop(screenW, screenH);
 
    /* Title */
    const char *title = "SUKUNA'S DOMAIN";   /* swap for your game's title */
    int titleSize = 46;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, screenW / 2 - titleW / 2, screenH / 2 - 170, titleSize, WHITE);
 
    const char *subtitle = "TAP TO BEGIN";
    int subSize = 18;
    int subW = MeasureText(subtitle, subSize);
    unsigned char alpha = (unsigned char)(150 + 80 * (0.5f + 0.5f * sinf(menu->titlePulse * 2.0f)));
    DrawText(subtitle, screenW / 2 - subW / 2, screenH / 2 - 110, subSize,
             (Color){ 200, 210, 230, alpha });
 
    /* Difficulty label + box position */
    Rectangle diffBox = { screenW / 2.0f - 150, screenH / 2.0f - 20, 300, 42 };
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiLabel((Rectangle){ diffBox.x, diffBox.y - 26, 300, 20 }, "Difficulty");
 
    Rectangle newGameBtn = { screenW / 2.0f - 150, screenH / 2.0f + 40, 300, 50 };
    Rectangle exitBtn    = { screenW / 2.0f - 150, screenH / 2.0f + 100, 300, 40 };
 
    if (!menu->difficultyDropdownOpen) {
        if (GuiButton(newGameBtn, "New Game")) menu->startPressed = true;
        if (GuiButton(exitBtn, "Exit"))         menu->exitPressed  = true;
    }
 
    int diffIndex = (int)menu->selectedDifficulty;
    if (GuiDropdownBox(diffBox, "Easy;Normal;Hard", &diffIndex, menu->difficultyDropdownOpen)) {
        menu->difficultyDropdownOpen = !menu->difficultyDropdownOpen;
    }
    menu->selectedDifficulty = (Difficulty)diffIndex;
}
 