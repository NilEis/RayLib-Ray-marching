/*******************************************************************************************
*
*   UniformsLayout v1.0.0 - Gui for uniforms
*
*   MODULE USAGE:
*       #define GUI_UNIFORMS_LAYOUT_IMPLEMENTATION
*       #include "gui_uniforms_layout.h"
*
*       INIT: GuiUniformsLayoutState state = InitGuiUniformsLayout();
*       DRAW: GuiUniformsLayout(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 Nils Eisenach. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#ifndef GUI_UNIFORMS_LAYOUT_H
#define GUI_UNIFORMS_LAYOUT_H

typedef struct {
    // Define anchors
    Vector2 anchor01;            // ANCHOR ID:1
    
    // Define controls variables
    bool WindowBox000Active;            // WindowBox: WindowBox000
    bool epsilon_inEditMode;
    float epsilon_inValue;            // Spinner: epsilon_in
    bool farplane_inEditMode;
    float farplane_inValue;            // Spinner: farplane_in
    bool max_steps_inEditMode;
    int max_steps_inValue;            // Spinner: max_steps_in

    // Define rectangles
    Rectangle layoutRecs[6];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiUniformsLayoutState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiUniformsLayoutState InitGuiUniformsLayout(void);
void GuiUniformsLayout(GuiUniformsLayoutState *state);


#ifdef __cplusplus
}
#endif

#endif // GUI_UNIFORMS_LAYOUT_H

/***********************************************************************************
*
*   GUI_UNIFORMS_LAYOUT IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_UNIFORMS_LAYOUT_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiUniformsLayoutState InitGuiUniformsLayout(void)
{
    GuiUniformsLayoutState state = { 0 };

    // Init anchors
    state.anchor01 = (Vector2){ 50, 96 };            // ANCHOR ID:1
    
    // Initilize controls variables
    state.WindowBox000Active = true;            // WindowBox: WindowBox000
    state.epsilon_inEditMode = false;
    state.epsilon_inValue = 0.0009;            // Spinner: epsilon_in
    state.farplane_inEditMode = false;
    state.farplane_inValue = 150;            // Spinner: farplane_in
    state.max_steps_inEditMode = false;
    state.max_steps_inValue = 250;            // Spinner: max_steps_in

    // Init controls rectangles
    state.layoutRecs[0] = (Rectangle){ state.anchor01.x + 0, state.anchor01.y + 0, 168, 120 };// WindowBox: WindowBox000
    state.layoutRecs[1] = (Rectangle){ state.anchor01.x + 60, state.anchor01.y + 30, 94, 20 };// Spinner: epsilon_in
    state.layoutRecs[2] = (Rectangle){ state.anchor01.x + 60, state.anchor01.y + 60, 94, 20 };// Spinner: farplane_in
    state.layoutRecs[3] = (Rectangle){ state.anchor01.x + 60, state.anchor01.y + 90, 94, 20 };// Spinner: max_steps_in
    state.layoutRecs[4] = (Rectangle){ state.anchor01.x + 1, state.anchor01.y + 50, 168, 10 };// Line: Line004
    state.layoutRecs[5] = (Rectangle){ state.anchor01.x + 0, state.anchor01.y + 80, 168, 10 };// Line: Line005

    // Custom variables initialization

    return state;
}


void GuiUniformsLayout(GuiUniformsLayoutState *state)
{
    // Const text
    const char *WindowBox000Text = "Uniforms";    // WINDOWBOX: WindowBox000
    
    // Draw controls
    if (state->WindowBox000Active)
    {
        state->WindowBox000Active = !GuiWindowBox(state->layoutRecs[0], WindowBox000Text);
        if (GuiSpinner(state->layoutRecs[1], "epsilon", &state->epsilon_inValue, 0, 100, state->epsilon_inEditMode)) state->epsilon_inEditMode = !state->epsilon_inEditMode;
        if (GuiSpinner(state->layoutRecs[2], "farplane", &state->farplane_inValue, 0, 100, state->farplane_inEditMode)) state->farplane_inEditMode = !state->farplane_inEditMode;
        if (GuiSpinner(state->layoutRecs[3], "max_steps", &state->max_steps_inValue, 0, 100, state->max_steps_inEditMode)) state->max_steps_inEditMode = !state->max_steps_inEditMode;
        GuiLine(state->layoutRecs[4], NULL);
        GuiLine(state->layoutRecs[5], NULL);
    }
}

#endif // GUI_UNIFORMS_LAYOUT_IMPLEMENTATION
