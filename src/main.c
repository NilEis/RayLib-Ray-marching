#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "video/video_main.h"
#include "video/shader.h"
#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_UNIFORMS_IMPLEMENTATION
#include "gui/uniforms_gui.h"

RenderTexture2D target;
Shader m_s;
static void get_input(void);
static void cleanup(void);

struct uniforms_t
{
	int t_loc;
	int EPSILON_loc;
	int FARPLANE_loc;
	int MAX_MARCH_STEPS_loc;
	int pos_loc;
	int view_loc;
	float t;
	float EPSILON_u;
	float FARPLANE_u;
	float MAX_MARCH_STEPS_u;
	float pos[3];
	float view_u[3];
} uniforms;
int main(int argc, char **argv)
{
	int width = 600;
	int height = 600;
	atexit(cleanup);
	video_init(width, height, "Shader");
	target = LoadRenderTexture(width, height);
	m_s = LoadShaderFromMemory(basic_shader_vs, basic_shader_fs);
	uniforms.t_loc = GetShaderLocation(m_s, "t");
	uniforms.t = 0;
	uniforms.EPSILON_loc = GetShaderLocation(m_s, "EPSILON");
	uniforms.EPSILON_u = 0.001;
	uniforms.FARPLANE_loc = GetShaderLocation(m_s, "FARPLANE");
	uniforms.FARPLANE_u = 5.0;
	uniforms.MAX_MARCH_STEPS_loc = GetShaderLocation(m_s, "MAX_MARCH_STEPS");
	uniforms.MAX_MARCH_STEPS_u = 250;
	uniforms.pos_loc = GetShaderLocation(m_s, "pos");
	uniforms.pos[0] = 0;
	uniforms.pos[1] = 0;
	uniforms.pos[2] = -2.5;
	SetShaderValue(m_s, uniforms.pos_loc, &uniforms.pos, SHADER_UNIFORM_VEC3);
	uniforms.view_loc = GetShaderLocation(m_s, "view");
	uniforms.view_u[0] = 0;
	uniforms.view_u[1] = 0;
	uniforms.view_u[2] = 1.0;
	SetShaderValue(m_s, uniforms.view_loc, &uniforms.view_u, SHADER_UNIFORM_VEC3);
	uniforms_gui_state_t state = uniforms_gui_init();
	state.update = true;
	while (!WindowShouldClose())
	{
		if (state.update == true)
		{
			uniforms.EPSILON_u = strtof(state.epsilon_inText, NULL);
			SetShaderValue(m_s, uniforms.EPSILON_loc, &uniforms.EPSILON_u, SHADER_UNIFORM_FLOAT);
			uniforms.FARPLANE_u = strtof(state.farplane_inText, NULL);
			SetShaderValue(m_s, uniforms.FARPLANE_loc, &uniforms.FARPLANE_u, SHADER_UNIFORM_FLOAT);
			uniforms.MAX_MARCH_STEPS_u = (int)strtol(state.max_steps_inText, NULL, 10);
			SetShaderValue(m_s, uniforms.MAX_MARCH_STEPS_loc, &uniforms.MAX_MARCH_STEPS_u, SHADER_UNIFORM_INT);
			state.update = false;
		}
		SetShaderValue(m_s, uniforms.t_loc, &uniforms.t, SHADER_UNIFORM_FLOAT);
		uniforms.t += 0.01;
		get_input();
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

static void get_input(void)
{
	const static float MOV_SPEED = 0.01;
	const static float ROT_SPEED = 0.01;
	static float v_rot = 0;
	if (IsKeyDown(KEY_W))
	{
		uniforms.pos[0] += uniforms.view_u[0] * MOV_SPEED;
		uniforms.pos[1] += uniforms.view_u[1] * MOV_SPEED;
		uniforms.pos[2] += uniforms.view_u[2] * MOV_SPEED;
		SetShaderValue(m_s, uniforms.pos_loc, &uniforms.pos, SHADER_UNIFORM_VEC3);
	}
	else if (IsKeyDown(KEY_S))
	{
		uniforms.pos[0] -= uniforms.view_u[0] * MOV_SPEED;
		uniforms.pos[1] -= uniforms.view_u[1] * MOV_SPEED;
		uniforms.pos[2] -= uniforms.view_u[2] * MOV_SPEED;
		SetShaderValue(m_s, uniforms.pos_loc, &uniforms.pos, SHADER_UNIFORM_VEC3);
	}
	if (IsKeyDown(KEY_A))
	{
		Vector3 tmp_view_u = {uniforms.view_u[0], uniforms.view_u[1], uniforms.view_u[2]};
		static const Vector3 ax = {0.0, 0.0, 1.0};
		Vector3 tmp_view = Vector3RotateByAxisAngle(tmp_view_u, ax, 0.1);
		uniforms.view_u[0] = tmp_view.x;
		uniforms.view_u[1] = tmp_view.y;
		uniforms.view_u[2] = tmp_view.z;
		SetShaderValue(m_s, uniforms.view_loc, &uniforms.view_u, SHADER_UNIFORM_VEC3);
	}
	else if (IsKeyDown(KEY_D))
	{
		Vector3 tmp_view = Vector3RotateByQuaternion((Vector3){uniforms.view_u[0], uniforms.view_u[1], uniforms.view_u[2]}, QuaternionFromAxisAngle((Vector3){1, 0, 0}, -ROT_SPEED));
		uniforms.view_u[0] = tmp_view.x;
		uniforms.view_u[1] = tmp_view.y;
		uniforms.view_u[2] = tmp_view.z;
		SetShaderValue(m_s, uniforms.view_loc, &uniforms.view_u, SHADER_UNIFORM_VEC3);
	}
}

static void cleanup(void)
{
	UnloadShader(m_s);
	UnloadRenderTexture(target);
	CloseWindow();
}