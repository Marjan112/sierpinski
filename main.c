#include <raylib.h>
#include <raymath.h>
#include <time.h>

#define MAX_POINTS 100000
#define POINTS_PER_FRAME 1000

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1600, 900, "Sierpinski");
    SetTargetFPS(60);
    SetRandomSeed(time(NULL));

    const Color LIGHT_BLUE = GetColor(0x89b4faff);
    const Color CHARCOAL_GREY = GetColor(0x1e1e2eff);

    const int w = GetScreenWidth();
    const int h = GetScreenHeight();

    const Vector2 p[] = {
        {w * 0.5, h * 0.2},
        {w * 0.2, h * 0.8},
        {w * 0.8, h * 0.8}
    };

    Vector2 vs[MAX_POINTS] = {0};
    size_t vs_count = 0;

    Vector2 v = p[0];

    while(!WindowShouldClose()) {
        if(vs_count < MAX_POINTS) {
            for(size_t i = 0; i < POINTS_PER_FRAME; ++i) {
                int p_index = GetRandomValue(0, 2);
                v = Vector2Lerp(v, p[p_index], 0.5);
                vs[vs_count++] = v;
            }
        }

        BeginDrawing();
        ClearBackground(CHARCOAL_GREY);

        DrawText("Press 'c' to clear the points", w * 0.01f, h * 0.01f, 30, LIGHT_BLUE);
        DrawText(TextFormat("Points: %d/%d", vs_count, MAX_POINTS), w * 0.01f, h * 0.05f, 30, LIGHT_BLUE);
        DrawFPS(w * 0.01f, h * 0.10f);

        if(IsKeyReleased(KEY_C)) vs_count = 0;

        DrawTriangleLines(p[0], p[1], p[2], LIGHT_BLUE);

        for(size_t i = 0; i < vs_count; ++i) DrawCircleV(vs[i], 0.5, LIGHT_BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
