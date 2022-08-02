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

RenderTexture2D render_tex;
RenderTexture2D pp_1_tex;
RenderTexture2D blur_tex;
RenderTexture2D hdr_tex;
RenderTexture2D gamma_tex;
Shader render_shader;
Shader pp_1_shader;
Shader blur_shader;
Shader hdr_shader;
Shader gamma_shader;
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
	render_tex = LoadRenderTexture(width, height);
	pp_1_tex = LoadRenderTexture(width, height);
	blur_tex = LoadRenderTexture(width, height);
	hdr_tex = LoadRenderTexture(width, height);
	gamma_tex = LoadRenderTexture(width, height);
	render_shader = LoadShaderFromMemory(basic_shader_vs, basic_shader_fs);
	pp_1_shader = LoadShaderFromMemory(NULL, pp_shader_1_fs);
	blur_shader = LoadShaderFromMemory(NULL, blur_shader_fs);
	hdr_shader = LoadShaderFromMemory(NULL, hdr_shader_fs);
	gamma_shader = LoadShaderFromMemory(NULL, gamma_shader_fs);
	uniforms.t_loc = GetShaderLocation(render_shader, "t");
	uniforms.t = 0;
	uniforms.EPSILON_loc = GetShaderLocation(render_shader, "EPSILON");
	uniforms.EPSILON_u = 0.001;
	uniforms.FARPLANE_loc = GetShaderLocation(render_shader, "FARPLANE");
	uniforms.FARPLANE_u = 5.0;
	uniforms.MAX_MARCH_STEPS_loc = GetShaderLocation(render_shader, "MAX_MARCH_STEPS");
	uniforms.MAX_MARCH_STEPS_u = 10;
	uniforms.pos_loc = GetShaderLocation(render_shader, "pos");
	uniforms.pos[0] = 0;
	uniforms.pos[1] = 0;
	uniforms.pos[2] = -2.5;
	SetShaderValue(render_shader, uniforms.pos_loc, &uniforms.pos, SHADER_UNIFORM_VEC3);
	uniforms.view_loc = GetShaderLocation(render_shader, "view");
	uniforms.view_u[0] = 0;
	uniforms.view_u[1] = 0;
	uniforms.view_u[2] = 1.0;
	SetShaderValue(render_shader, uniforms.view_loc, &uniforms.view_u, SHADER_UNIFORM_VEC3);
	uniforms_gui_state_t state = uniforms_gui_init();
	state.update = true;
	while (!WindowShouldClose())
	{
		if (state.update == true)
		{
			uniforms.EPSILON_u = strtof(state.epsilon_inText, NULL);
			SetShaderValue(render_shader, uniforms.EPSILON_loc, &uniforms.EPSILON_u, SHADER_UNIFORM_FLOAT);
			uniforms.FARPLANE_u = strtof(state.farplane_inText, NULL);
			SetShaderValue(render_shader, uniforms.FARPLANE_loc, &uniforms.FARPLANE_u, SHADER_UNIFORM_FLOAT);
			uniforms.MAX_MARCH_STEPS_u = (int)strtol(state.max_steps_inText, NULL, 10);
			SetShaderValue(render_shader, uniforms.MAX_MARCH_STEPS_loc, &uniforms.MAX_MARCH_STEPS_u, SHADER_UNIFORM_INT);
			state.update = false;
		}
		SetShaderValue(render_shader, uniforms.t_loc, &uniforms.t, SHADER_UNIFORM_FLOAT);
		uniforms.t += 0.01;
		get_input();
		BeginTextureMode(render_tex);
		{
			BeginShaderMode(render_shader);
			{
				DrawTexturePro(gamma_tex.texture, (Rectangle){0, 0, (float)gamma_tex.texture.width, (float)-gamma_tex.texture.height}, (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, (Vector2){0, 0}, 0.0, WHITE);
			}
			EndShaderMode();
		}
		EndTextureMode();
		BeginTextureMode(pp_1_tex);
		{
			BeginShaderMode(pp_1_shader);
			{
				DrawTexturePro(render_tex.texture, (Rectangle){0, 0, (float)render_tex.texture.width, (float)-render_tex.texture.height}, (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, (Vector2){0, 0}, 0.0, WHITE);
			}
			EndShaderMode();
		}
		BeginTextureMode(blur_tex);
		{
			BeginShaderMode(blur_shader);
			{
				DrawTexturePro(pp_1_tex.texture, (Rectangle){0, 0, (float)pp_1_tex.texture.width, (float)-pp_1_tex.texture.height}, (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, (Vector2){0, 0}, 0.0, WHITE);
			}
			EndShaderMode();
		}
		BeginTextureMode(hdr_tex);
		{
			BeginShaderMode(hdr_shader);
			{
				DrawTexturePro(blur_tex.texture, (Rectangle){0, 0, (float)blur_tex.texture.width, (float)-blur_tex.texture.height}, (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, (Vector2){0, 0}, 0.0, WHITE);
			}
			EndShaderMode();
		}
		BeginTextureMode(gamma_tex);
		{
			BeginShaderMode(gamma_shader);
			{
				DrawTexturePro(hdr_tex.texture, (Rectangle){0, 0, (float)hdr_tex.texture.width, (float)-hdr_tex.texture.height}, (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, (Vector2){0, 0}, 0.0, WHITE);
			}
			EndShaderMode();
		}
		EndTextureMode();
		BeginDrawing();
		{
			ClearBackground(BLACK);
			BeginShaderMode(gamma_shader);
			{
				DrawTexturePro(gamma_tex.texture, (Rectangle){0, 0, (float)gamma_tex.texture.width, (float)-gamma_tex.texture.height}, (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, (Vector2){0, 0}, 0.0, WHITE);
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
		SetShaderValue(render_shader, uniforms.pos_loc, &uniforms.pos, SHADER_UNIFORM_VEC3);
	}
	else if (IsKeyDown(KEY_S))
	{
		uniforms.pos[0] -= uniforms.view_u[0] * MOV_SPEED;
		uniforms.pos[1] -= uniforms.view_u[1] * MOV_SPEED;
		uniforms.pos[2] -= uniforms.view_u[2] * MOV_SPEED;
		SetShaderValue(render_shader, uniforms.pos_loc, &uniforms.pos, SHADER_UNIFORM_VEC3);
	}
	if (IsKeyDown(KEY_A))
	{
		Vector3 tmp_view_u = {uniforms.view_u[0], uniforms.view_u[1], uniforms.view_u[2]};
		static const Vector3 ax = {0.0, 0.0, 1.0};
		Vector3 tmp_view = Vector3RotateByAxisAngle(tmp_view_u, ax, 0.1);
		uniforms.view_u[0] = tmp_view.x;
		uniforms.view_u[1] = tmp_view.y;
		uniforms.view_u[2] = tmp_view.z;
		SetShaderValue(render_shader, uniforms.view_loc, &uniforms.view_u, SHADER_UNIFORM_VEC3);
	}
	else if (IsKeyDown(KEY_D))
	{
		Vector3 tmp_view = Vector3RotateByQuaternion((Vector3){uniforms.view_u[0], uniforms.view_u[1], uniforms.view_u[2]}, QuaternionFromAxisAngle((Vector3){1, 0, 0}, -ROT_SPEED));
		uniforms.view_u[0] = tmp_view.x;
		uniforms.view_u[1] = tmp_view.y;
		uniforms.view_u[2] = tmp_view.z;
		SetShaderValue(render_shader, uniforms.view_loc, &uniforms.view_u, SHADER_UNIFORM_VEC3);
	}
}

static void cleanup(void)
{
	UnloadShader(render_shader);
	UnloadShader(pp_1_shader);
	UnloadShader(blur_shader);
	UnloadShader(hdr_shader);
	UnloadShader(gamma_shader);
	UnloadRenderTexture(render_tex);
	UnloadRenderTexture(pp_1_tex);
	UnloadRenderTexture(hdr_tex);
	UnloadRenderTexture(gamma_tex);
	CloseWindow();
}