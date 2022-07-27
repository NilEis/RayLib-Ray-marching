#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "video/video_main.h"
#include "video/shader.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_UNIFORMS_IMPLEMENTATION
#include "gui/uniforms_gui.h"

RenderTexture2D target;
Shader m_s;
static void cleanup(void);
int main(int argc, char **argv)
{
	int width = 1080;
	int height = 1080;
	atexit(cleanup);
	video_init(width, height, "Shader");
	target = LoadRenderTexture(width, height);
	m_s = LoadShaderFromMemory(basic_shader_vs, basic_shader_fs);
	int t_loc = GetShaderLocation(m_s, "t");
	float t = 0;
	int EPSILON_loc = GetShaderLocation(m_s, "EPSILON");
	float EPSILON = 0.09;
	int FARPLANE_loc = GetShaderLocation(m_s, "FARPLANE");
	float FARPLANE = 5.0;
	int MAX_MARCH_STEPS_loc = GetShaderLocation(m_s, "MAX_MARCH_STEPS");
	float MAX_MARCH_STEPS = 250;
	uniforms_gui_state_t state = uniforms_gui_init();
	while (!WindowShouldClose())
	{
		if (state.update == true)
		{
			EPSILON = strtof(state.epsilon_inText, NULL);
			SetShaderValue(m_s, EPSILON_loc, &EPSILON, SHADER_UNIFORM_FLOAT);
			FARPLANE = strtof(state.farplane_inText, NULL);
			SetShaderValue(m_s, FARPLANE_loc, &FARPLANE, SHADER_UNIFORM_FLOAT);
			MAX_MARCH_STEPS = (int)strtol(state.max_steps_inText, NULL, 10);
			SetShaderValue(m_s, MAX_MARCH_STEPS_loc, &MAX_MARCH_STEPS, SHADER_UNIFORM_INT);
			state.update = false;
		}
		SetShaderValue(m_s, t_loc, &t, SHADER_UNIFORM_FLOAT);
		t += 0.01;
		BeginTextureMode(target);
		{
			ClearBackground(BLACK);
		}
		EndTextureMode();
		BeginDrawing();
		{
			ClearBackground(BLACK);
			BeginShaderMode(m_s);
			{
				DrawTexturePro(target.texture, (Rectangle){0, 0, (float)target.texture.width, (float)-target.texture.height}, (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, (Vector2){0, 0}, 0.0, WHITE);
			}
			EndShaderMode();
			uniforms_gui_draw(&state);
			DrawFPS(10, 10);
		}
		EndDrawing();
	}
}

static void cleanup(void)
{
	UnloadShader(m_s);
	UnloadRenderTexture(target);
	CloseWindow();
}
