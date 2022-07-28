/*******************************************************************************************
*
*   UniformsGui v1.0.0 - Tool description
*   The gui to change the uniforms
*
*   MODULE USAGE:
*       #define GUI_UNIFORMS_GUI_IMPLEMENTATION
*       #include "gui_uniforms_gui.h"
*
*       INIT: uniforms_gui_state_t state = uniforms_gui_init();
*       DRAW: uniforms_gui_draw(&state);
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

#ifndef GUI_UNIFORMS_GUI_H
#define GUI_UNIFORMS_GUI_H

typedef struct
{
    // Define anchors
    Vector2 anchor01; // ANCHOR ID:1

    // Define controls variables
    bool uniforms_boxActive; // WindowBox: uniforms_box
    bool epsilon_inEditMode;
    char epsilon_inText[128]; // TextBox: epsilon_in
    bool farplane_inEditMode;
    char farplane_inText[128]; // TextBox: farplane_in
    bool max_steps_inEditMode;
    char max_steps_inText[128]; // TextBox: max_steps_in

    // Define rectangles
    Rectangle layoutRecs[11];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required
    bool update;

} uniforms_gui_state_t;

#ifdef __cplusplus
extern "C"
{ // Prevents name mangling of functions
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
    uniforms_gui_state_t uniforms_gui_init(void);
    void uniforms_gui_draw(uniforms_gui_state_t *state);
    static void UpdateButton(); // Button: update_button logic

#ifdef __cplusplus
}
#endif

#endif // GUI_UNIFORMS_GUI_H

/***********************************************************************************
*
*   GUI_UNIFORMS_GUI IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_UNIFORMS_IMPLEMENTATION)

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
uniforms_gui_state_t uniforms_gui_init(void)
{
    uniforms_gui_state_t state = {0};

    // Init anchors
    state.anchor01 = (Vector2){5, 100}; // ANCHOR ID:1

    // Initilize controls variables
    state.uniforms_boxActive = true; // WindowBox: uniforms_box
    state.epsilon_inEditMode = false;
    strcpy(state.epsilon_inText, "0.001"); // TextBox: epsilon_in
    state.farplane_inEditMode = false;
    strcpy(state.farplane_inText, "150.0"); // TextBox: farplane_in
    state.max_steps_inEditMode = false;
    strcpy(state.max_steps_inText, "250"); // TextBox: max_steps_in

    // Init controls rectangles
    state.layoutRecs[0] = (Rectangle){state.anchor01.x + 0, state.anchor01.y + 0, 167, 192};   // WindowBox: uniforms_box
    state.layoutRecs[1] = (Rectangle){state.anchor01.x + 1, state.anchor01.y + 24, 96, 24};    // Label: epsilon_label
    state.layoutRecs[2] = (Rectangle){state.anchor01.x + 1, state.anchor01.y + 47, 165, 24};   // Line: Line002
    state.layoutRecs[3] = (Rectangle){state.anchor01.x + 95, state.anchor01.y + 24, 71, 24};   // TextBox: epsilon_in
    state.layoutRecs[4] = (Rectangle){state.anchor01.x + 1, state.anchor01.y + 95, 165, 24};   // Line: Line004
    state.layoutRecs[5] = (Rectangle){state.anchor01.x + 1, state.anchor01.y + 71, 96, 24};    // Label: farplane_label
    state.layoutRecs[6] = (Rectangle){state.anchor01.x + 96, state.anchor01.y + 71, 71, 24};   // TextBox: farplane_in
    state.layoutRecs[7] = (Rectangle){state.anchor01.x + 1, state.anchor01.y + 119, 96, 24};   // Label: max_steps_label
    state.layoutRecs[8] = (Rectangle){state.anchor01.x + 95, state.anchor01.y + 119, 71, 24};  // TextBox: max_steps_in
    state.layoutRecs[9] = (Rectangle){state.anchor01.x + 1, state.anchor01.y + 144, 165, 24};  // Line: Line009
    state.layoutRecs[10] = (Rectangle){state.anchor01.x + 1, state.anchor01.y + 168, 168, 24}; // Button: update_button

    // Custom variables initialization
    state.update = false;

    return state;
}

void uniforms_gui_draw(uniforms_gui_state_t *state)
{
    // Const text
    const char *uniforms_boxText = "Uniforms";     // WINDOWBOX: uniforms_box
    const char *epsilon_labelText = " Epsilon";    // LABEL: epsilon_label
    const char *farplane_labelText = "Farplane";   // LABEL: farplane_label
    const char *max_steps_labelText = "Max steps"; // LABEL: max_steps_label
    const char *update_buttonText = "Update";      // BUTTON: update_button

    // Draw controls
    if (state->uniforms_boxActive)
    {
        state->uniforms_boxActive = !GuiWindowBox(state->layoutRecs[0], uniforms_boxText);
        GuiLabel(state->layoutRecs[1], epsilon_labelText);
        GuiLine(state->layoutRecs[2], NULL);
        if (GuiTextBox(state->layoutRecs[3], state->epsilon_inText, 128, state->epsilon_inEditMode))
            state->epsilon_inEditMode = !state->epsilon_inEditMode;
        GuiLine(state->layoutRecs[4], NULL);
        GuiLabel(state->layoutRecs[5], farplane_labelText);
        if (GuiTextBox(state->layoutRecs[6], state->farplane_inText, 128, state->farplane_inEditMode))
            state->farplane_inEditMode = !state->farplane_inEditMode;
        GuiLabel(state->layoutRecs[7], max_steps_labelText);
        if (GuiTextBox(state->layoutRecs[8], state->max_steps_inText, 128, state->max_steps_inEditMode))
            state->max_steps_inEditMode = !state->max_steps_inEditMode;
        GuiLine(state->layoutRecs[9], NULL);
        if (GuiButton(state->layoutRecs[10], update_buttonText))
            state->update = true;
    }
}

#endif // GUI_UNIFORMS_GUI_IMPLEMENTATION
