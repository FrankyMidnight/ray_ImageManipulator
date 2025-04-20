#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

int WIN_WIDTH = 120;
int WIN_HEIGHT = 120;
const char *WIN_TITLE = "Generate bands";

bool setup(void);
void cleanup(void);
void update(void);
void draw(void);

// unsigned char *pixels = {NULL};
Color *pixels = {NULL};
const int CHANNELS = 4;
enum
{
    RED_OFFSET = 0,
    GREEN_OFFSET,
    BLUE_OFFSET,
    ALPHA_OFFSET
};

Image base_image;
Image copy_image;
Texture2D texture;


void generate_vBands(void);
void generate_hBands(void);
void generate_checkerBands(void);

int main(void)
{
    if (!setup())
    {
        fprintf(stderr, "Errore in setup esco ciao...\n");
        return EXIT_FAILURE;
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        update();
        draw();
        EndDrawing();
    }
    cleanup();
    return 0;
}

bool setup(void)
{
    // WINDOW
    InitWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
    if (!IsWindowReady())
    {
        fprintf(stderr, "Errore in creazione finestra\n");
        return false;
    }
    // // PIXELS BUFFER
    // pixels = calloc( (size_t)(WIN_WIDTH * WIN_HEIGHT * CHANNELS), sizeof(unsigned char));
    // if(!pixels)
    // {
    //     fprintf(stderr, "Errore in allocazione pixels buffer\n");
    //     return false;
    // }
    // GENERATE IMAGE
    base_image = GenImageColor(WIN_WIDTH, WIN_HEIGHT, RED);
    ImageFormat(&base_image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    copy_image = ImageCopy(base_image);
    pixels = LoadImageColors(base_image);
    if (!IsImageValid(base_image) || !IsImageValid(copy_image))
    {
        fprintf(stderr, "Errore in creazione immagine\n");
        return false;
    }
    // LOAD BASE IN TEXTURE
    texture = LoadTextureFromImage(base_image);
    if (!IsTextureValid(texture))
    {
        fprintf(stderr, "Errore in creazione texture\n");
        return false;
    }
    return true;
}
void cleanup(void)
{
    UnloadTexture(texture);
    // free(pixels);
    UnloadImage(base_image);
    UnloadImage(copy_image);
    CloseWindow();
}
void update(void)
{
    if (IsKeyPressed(KEY_V))
    {
        generate_vBands();
    }
    if (IsKeyPressed(KEY_H))
    {
        generate_hBands();
    }
    if (IsKeyPressed(KEY_C))
    {
        generate_checkerBands();
    }
}
void draw(void)
{
    ClearBackground(DARKGRAY);
    DrawTexture(texture, 0, 0, WHITE);
}

void generate_vBands(void)
{
    
    int band_height = WIN_HEIGHT / 4;
    bool switch_color = false;
    for (int row = 0; row < WIN_HEIGHT; ++row)
    {
        // if (row % band_height == 0)
        // {
        //     switch_color = !switch_color;
        // }
        for (int col = 0; col < WIN_WIDTH; ++col)
        {
            if (col % band_height == 0)
            {
                switch_color = !switch_color;
            }
            int i = row * WIN_WIDTH + col;
            if (switch_color)
            {
                
                pixels[i] = GREEN;
            }
            else if (!switch_color)
            {
                
                pixels[i] = RED;
            }
        }
    }
    UpdateTexture(texture, pixels);
}
void generate_hBands(void)
{
    
    int band_height = WIN_HEIGHT / 4;
    bool switch_color = false;
    for (int row = 0; row < WIN_HEIGHT; ++row)
    {
        if (row % band_height == 0)
        {
            switch_color = !switch_color;
        }
        for (int col = 0; col < WIN_WIDTH; ++col)
        {
            // if (col % band_height == 0)
            // {
            //     switch_color = !switch_color;
            // }
            int i = row * WIN_WIDTH + col;
            if (switch_color)
            {
                
                pixels[i] = GREEN;
            }
            else if (!switch_color)
            {
                
                pixels[i] = RED;
            }
        }
    }
    UpdateTexture(texture, pixels);
}
void generate_checkerBands(void)
{
    
    int band_height = WIN_HEIGHT / 4;
    bool switch_color = false;
    for (int row = 0; row < WIN_HEIGHT; ++row)
    {
        if (row % band_height == 0)
        {
            switch_color = !switch_color;
        }
        for (int col = 0; col < WIN_WIDTH; ++col)
        {
            if (col % band_height == 0)
            {
                switch_color = !switch_color;
            }
            int i = row * WIN_WIDTH + col;
            if (switch_color)
            {
                
                pixels[i] = GREEN;
            }
            else if (!switch_color)
            {
                
                pixels[i] = RED;
            }
        }
    }
    UpdateTexture(texture, pixels);
}