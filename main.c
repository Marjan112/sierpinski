#include <raylib.h>
#include <raymath.h>
#include <time.h>

#define POINTS_PER_FRAME 1000

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1600, 900, "Sierpinski");
    SetTargetFPS(60);
    SetRandomSeed(time(NULL));

    Camera2D camera = {0};
    camera.zoom = 1;

    const Color LIGHT_BLUE = GetColor(0x89b4faff);
    const Color CHARCOAL_GREY = GetColor(0x1e1e2eff);

    const int w = GetScreenWidth();
    const int h = GetScreenHeight();

    // CCW
    const Vector2 p[] = {
        {w * 0.5f, h * 0.2f},
        {w * 0.2f, h * 0.8f},
        {w * 0.8f, h * 0.8f}
    };

    Vector2 point;
    Vector2 seed = p[0];

    while(!WindowShouldClose()) {
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        const float wheel = GetMouseWheelMove();
        if(wheel != 0) {
            Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.offset = GetMousePosition();
            camera.target = mouse_world_pos;
            camera.zoom *= (1 + (wheel * 0.05f));
            camera.zoom = Clamp(camera.zoom, 1, 30000);
        }

        Vector2 top_left = GetScreenToWorld2D(CLITERAL(Vector2){0, 0}, camera);
        Vector2 bottom_right = GetScreenToWorld2D(CLITERAL(Vector2){GetScreenWidth(), GetScreenHeight()}, camera);

        Rectangle view_area = {
            top_left.x,
            top_left.y,
            bottom_right.x - top_left.x,
            bottom_right.y - top_left.y
        };

        for(int i = 0; i < 100; ++i) seed = Vector2Lerp(seed, p[GetRandomValue(0, 2)], 0.5f);

        if(IsKeyDown(KEY_Z)) camera.zoom = 1;
        if(IsKeyDown(KEY_S)) TakeScreenshot("screenshot.png");

        BeginDrawing(); {
            ClearBackground(CHARCOAL_GREY);

            BeginMode2D(camera);
            DrawTriangleLines(p[0], p[1], p[2], LIGHT_BLUE);
            EndMode2D();

            Vector2 ephemeral = seed;
            size_t points_to_draw = (POINTS_PER_FRAME * camera.zoom);
            points_to_draw = Clamp(points_to_draw, POINTS_PER_FRAME, 2000000);
            for(size_t i = 0; i < points_to_draw; ++i) {
                ephemeral = Vector2Lerp(ephemeral, p[GetRandomValue(0, 2)], 0.5f);
                if(CheckCollisionPointRec(ephemeral, view_area)) {
                    Vector2 screen_pos = GetWorldToScreen2D(ephemeral, camera);
                    DrawCircleV(screen_pos, 1.5f, LIGHT_BLUE);
                }
            }

            seed = ephemeral;

            DrawText("Press 'z' to reset the zoom", w * 0.01f, h * 0.03f, 30, LIGHT_BLUE);
            DrawText("Press 's' for screenshot", w * 0.01f, h * 0.07f, 30, LIGHT_BLUE);
            DrawFPS(w * 0.01f, h * 0.11f);
            DrawText(TextFormat("Zoom: %.2fx", camera.zoom), w * 0.01f, h * 0.15f, 30, LIGHT_BLUE);
        } EndDrawing();
    }

    CloseWindow();
    return 0;
}
