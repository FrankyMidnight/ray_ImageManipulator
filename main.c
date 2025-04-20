#define RAYGUI_IMPLEMENTATION

#include <raygui.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int WIN_WIDTH = 800;
int WIN_HEIGHT = 600;

const char *WIN_TITLE = "ray_ImageManipulator";
char filepath[256];

const float MIN_SLIDER = -30.0f;
const float MAX_SLIDER = 30.0f;

float input_red = 0;
float input_green = 0;
float input_blue = 0;
float input_brightness = 0;
float input_contrast = 0;

float old_input_red = 0;
float old_input_green = 0;
float old_input_blue = 0;
float old_input_brightness = 0;
float old_input_contrast = 0;

bool set_grayscale = false;
bool set_reset = false;

Image image_original;
Image image_copy;
Texture2D texture;
Color *pixels;

typedef enum
{
    COLOR_R = 0,
    COLOR_G,
    COLOR_B,
    COLOR_A,
} color_t;

bool setup();
void read_input();
void draw();
void cleanup();

int clamp(int value);
void add_color(int value, color_t color);
void brightness(int value);
void contrast(int value);
void grayscale();
void reset();

int main(int argc, char *argv[])
{
    // Read file
    if (!argv[1])
    {
        TraceLog(LOG_ERROR, "Usage : ./ray_ImageManipulator SOME_IMAGEFILE.png");
        return EXIT_SUCCESS;
    }
    strncpy(filepath, argv[1], 256);
    if (!setup())
    {
        return EXIT_FAILURE;
        fprintf(stderr, "Errore in setup..\n");
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        draw();
        read_input();
        EndDrawing();
    }
    cleanup();
    return 0;
}

bool setup()
{
    // Init window
    InitWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
    if (!IsWindowReady())
    {
        TraceLog(LOG_ERROR, "Error initializing window");
        return false;
    }
    // Load GUI style
    GuiLoadStyle("style_cyber.rgs");
    // Load image for color add
    image_original = LoadImage(filepath);
    if (!IsImageValid(image_original))
    {
        TraceLog(LOG_ERROR, "Error loading image");
        return false;
    }
    // format to 32-bit in order to render texture
    ImageFormat(&image_original, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    // copy image
    image_copy = ImageCopy(image_original);
    // load texture from image
    texture = LoadTextureFromImage(image_original);
    if (!IsTextureValid(texture))
    {
        TraceLog(LOG_ERROR, "Error loading texture");
        UnloadImage(image_copy);
        UnloadImage(image_original);
        return false;
    }
    // carico i pixel da modificare
    pixels = LoadImageColors(image_copy);
    return true;
}
void cleanup()
{
    UnloadImageColors(pixels);
    UnloadImage(image_original);
    UnloadImage(image_copy);
    UnloadTexture(texture);
    CloseWindow();
}
void draw()
{
    ClearBackground(DARKGRAY);
    // draw texture
    DrawTexture(texture, 0, 0, WHITE);

    // DRAW GUI

    // ADD RED
    GuiLabel((Rectangle){.x = WIN_WIDTH - 300, .y = 0, .width = 100, .height = 30}, "RED");
    GuiSlider((Rectangle){.x = WIN_WIDTH - 150, .y = 0, .width = 100, .height = 30}, NULL, NULL, &input_red, MIN_SLIDER,
              MAX_SLIDER);

    // ADD GREEN
    GuiLabel((Rectangle){.x = WIN_WIDTH - 300, .y = 30, .width = 100, .height = 30}, "GREEN");
    GuiSlider((Rectangle){.x = WIN_WIDTH - 150, .y = 30, .width = 100, .height = 30}, NULL, NULL, &input_green,
              MIN_SLIDER, MAX_SLIDER);

    // ADD BLUE
    GuiLabel((Rectangle){.x = WIN_WIDTH - 300, .y = 60, .width = 100, .height = 30}, "BLUE");
    GuiSlider((Rectangle){.x = WIN_WIDTH - 150, .y = 60, .width = 100, .height = 30}, NULL, NULL, &input_blue,
              MIN_SLIDER, MAX_SLIDER);

    // ADD BRIGHTNESS
    GuiLabel((Rectangle){.x = WIN_WIDTH - 300, .y = 90, .width = 100, .height = 30}, "BRIGHTNESS");
    GuiSlider((Rectangle){.x = WIN_WIDTH - 150, .y = 90, .width = 100, .height = 30}, NULL, NULL, &input_brightness,
              MIN_SLIDER, MAX_SLIDER);

    // ADD CONTRAST
    GuiLabel((Rectangle){.x = WIN_WIDTH - 300, .y = 120, .width = 100, .height = 30}, "CONTRAST");
    GuiSlider((Rectangle){.x = WIN_WIDTH - 150, .y = 120, .width = 100, .height = 30}, NULL, NULL, &input_contrast,
              MIN_SLIDER, MAX_SLIDER);

    // SET GRAYSCALE
    set_grayscale = GuiButton((Rectangle){.x = WIN_WIDTH - 300, .y = 170, .width = 100, .height = 30}, "GRAYSCALE");

    // SET RESET
    set_reset = GuiButton((Rectangle){.x = WIN_WIDTH - 150, .y = 170, .width = 100, .height = 30}, "RESET");
}
void read_input()
{
    if (old_input_red != input_red)
    {
        add_color((int)input_red, COLOR_R);
        old_input_red = input_red;
    }
    if (old_input_green != input_green)
    {
        add_color((int)input_green, COLOR_G);
        old_input_green = input_green;
    }
    if (old_input_blue != input_blue)
    {
        add_color((int)input_blue, COLOR_B);
        old_input_blue = input_blue;
    }
    if (old_input_brightness != input_brightness)
    {
        brightness((int)input_brightness);
        old_input_brightness = input_brightness;
    }
    if (old_input_contrast != input_contrast)
    {
        contrast((int)input_contrast);
        old_input_contrast = input_contrast;
    }
    if (IsKeyPressed(KEY_F12))
    {
        char *path_screenshot = "Screenshot.png";
        TakeScreenshot(path_screenshot);
    }
    if (set_grayscale)
    {
        grayscale();
    }
    if (set_reset)
    {
        reset();
    }
}
int clamp(int value)
{
    // max(minimum, min(x, maximum))
    // max(0, min(value , 255))
    int minimum = (value < 255) ? value : 255;
    int maximum = (0 > minimum) ? 0 : minimum;
    return maximum;
}
void add_color(int value, color_t color)
{
    switch (color)
    {
    case COLOR_R:
        for (int i = 0; i < image_copy.height * image_copy.width; i++)
        {
            pixels[i].r = clamp(pixels[i].r + value);
        }
        break;
    case COLOR_G:
        for (int i = 0; i < image_copy.height * image_copy.width; i++)
        {
            pixels[i].g = clamp(pixels[i].g + value);
        }
        break;
    case COLOR_B:
        for (int i = 0; i < image_copy.height * image_copy.width; i++)
        {
            pixels[i].b = clamp(pixels[i].b + value);
        }
        break;
    default:
        break;
    }

    UpdateTexture(texture, pixels);
}
void grayscale()
{
    for (int i = 0; i < image_copy.height * image_copy.width; i++)
    {
        int mean = (pixels[i].r + pixels[i].g + pixels[i].b) / 3;
        pixels[i].r = mean;
        pixels[i].g = mean;
        pixels[i].b = mean;
    }
    UpdateTexture(texture, pixels);
}
void brightness(int brightness)
{
    for (int i = 0; i < image_copy.height * image_copy.width; i++)
    {
        int red = pixels[i].r + brightness;
        pixels[i].r = clamp(red);

        int green = pixels[i].g + brightness;
        pixels[i].g = clamp(green);

        int blue = pixels[i].b + brightness;
        pixels[i].b = clamp(blue);
    }
    UpdateTexture(texture, pixels);
}
void contrast(int contrast)
{
    const float alpha = (contrast + 255) / 255.0f;
    for (int i = 0; i < image_copy.height * image_copy.width; i++)
    {
        int red = pixels[i].r;
        int green = pixels[i].g;
        int blue = pixels[i].b;

        red = alpha * (red - 128) + 128;
        red = clamp(red);

        green = alpha * (green - 128) + 128;
        green = clamp(green);

        blue = alpha * (blue - 128) + 128;
        blue = clamp(blue);

        pixels[i].r = red;
        pixels[i].g = green;
        pixels[i].b = blue;
    }
    UpdateTexture(texture, pixels);
}
void reset()
{
    // copy image
    image_copy = ImageCopy(image_original);
    // load texture from image
    texture = LoadTextureFromImage(image_original);
    // carico i pixel da modificare
    pixels = LoadImageColors(image_copy);
    input_red = 0.0f;
    input_green = 0.0f;
    input_blue = 0.0f;
    old_input_red = 0.0f;
    old_input_green = 0.0f;
    old_input_blue = 0.0f;

    input_brightness = 0.0f;
    input_contrast = 0.0f;
    old_input_brightness = 0.0f;
    old_input_contrast = 0.0f;
}
