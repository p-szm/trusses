//
//  button.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 02/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "button.h"
#include "interface.h"
#include "save.h"
#include "selection_tool.h"
#include "obstacle_tool.h"
#include "trace_tool.h"
#include "bars_tool.h"
#include "renderer.h"
#include "window.h"
#include "game.h"
#include "settings.h"

std::vector<Button> buttons;

Button::Button(double w, double h, double pos_x, double pos_y, int off_x, int off_y, void (*a)(void), std::string t)
{
    width_ = w;
    height_ = h;
    position = Vector2d(pos_x, pos_y);
    offset = Vector2d(off_x, off_y);
    
    action = a;
    highlighted_ = false;
    text_ = t;
    active_ = false;
    change_state_ = false;
    
    id_ = (int)buttons.size();
}

int Button::create(double w, double h, double pos_x, double pos_y, int off_x, int off_y, void (*a)(void), std::string t)
{
    Button b = Button(w, h, pos_x, pos_y, off_x, off_y, a, t);
    buttons.push_back(b);
    return b.id_;
}

bool Button::is_hit(double x, double y)
{
    double left = position.x + px_to_ui_x(offset.x - width_ / 2.0);
    double right = position.x + px_to_ui_x(offset.x + width_ / 2.0);
    double top = position.y + px_to_ui_y(offset.y + height_ / 2.0);
    double bottom = position.y + px_to_ui_y(offset.y - height_ / 2.0);
    
    if (x >= left && x <= right && y <= top && y >= bottom)
        return true;
    else
        return false;
}

void Button::execute_action()
{
    action();
    if (change_state_)
        active_ = !active_;
}

void Button::draw(const Renderer& rend) const
{
    rend.render(*this);
}

void button_ids_action(void)
{
    settings.toggle(IDS);
}

void button_lengths_action(void)
{
    settings.toggle(LENGTHS);
}

void button_rel_extensions_action(void)
{
    settings.toggle(EXTENSIONS);
}

void button_reset_action(void)
{
    game.reset();
}

void button_save_action(void)
{
    std::string path = "/Users/patrick/Desktop/save-";
    path += date_str() + '-' + time_str();
    save(path);
}

void button_scale_up_action(void)
{
    window.scale *= 1.1;
}

void button_scale_down_action(void)
{
    window.scale /= 1.1;
}

void button_selection_action(void)
{
    Tool::set(current_tool, new SelectionTool);
}

void button_obstacle_action(void)
{
    Tool::set(current_tool, new ObstacleTool);
}

void button_triangulation_action(void)
{
    settings.toggle(TRIANGULATION);
}

void button_bboxes_action(void)
{
    settings.toggle(BOUNDING_BOXES);
}

void button_enter_simulation(void)
{
    game.enter_simulation();
}

void button_enter_editor(void)
{
    game.enter_editor();
}

void button_bars_tool(void)
{
    Tool::set(current_tool, new BarsTool);
}

void button_trace_tool(void)
{
    Tool::set(current_tool, new TraceTool);
}

void create_buttons_editor()
{
    const int tmargin = 40;
    const int lmargin = 50;
    const int w = 60;
    const int h = 30;
    const int dy = 40;
    
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin, &button_save_action, "Save");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-dy, &button_reset_action, "Reset");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-2.5*dy, &button_ids_action, "Ids");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-3.5*dy, &button_lengths_action, "Lengths");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-4.5*dy, &button_rel_extensions_action, "Rel ext");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-5.5*dy, &button_triangulation_action, "Triangles");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-6.5*dy, &button_bboxes_action, "B-boxes");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-8*dy, &button_scale_up_action, "     +");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-9*dy, &button_scale_down_action, "     -");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-10.5*dy, &button_enter_simulation, "Simulate");
    
    Button::create(w, h, 1.0, 1.0, -lmargin, -tmargin, &button_bars_tool, "Draw");
    Button::create(w, h, 1.0, 1.0, -lmargin, -tmargin-dy, &button_selection_action, "Select");
    Button::create(w, h, 1.0, 1.0, -lmargin, -tmargin-2*dy, &button_obstacle_action, "Obstacle");
    Button::create(w, h, 1.0, 1.0, -lmargin, -tmargin-3*dy, &button_trace_tool, "Trace");
}

void create_buttons_simulation()
{
    const int tmargin = 40;
    const int lmargin = 50;
    const int w = 60;
    const int h = 30;
    const int dy = 40;
    
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin, &button_save_action, "Save");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-dy, &button_reset_action, "Reset");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-2.5*dy, &button_ids_action, "Ids");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-3.5*dy, &button_lengths_action, "Lengths");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-4.5*dy, &button_rel_extensions_action, "Rel ext");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-5.5*dy, &button_triangulation_action, "Triangles");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-6.5*dy, &button_bboxes_action, "B-boxes");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-8*dy, &button_scale_up_action, "     +");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-9*dy, &button_scale_down_action, "     -");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-10.5*dy, &button_enter_editor, "Edit");
}

void highlight_buttons(double x, double y)
{
    bool button_hit = false;
    for (int i = 0; i < buttons.size() && button_hit == false; i++)
    {
        if (buttons[i].is_hit(x, y))
        {
            buttons[i].highlighted_ = true;
            button_hit = true;
        }
        else
            buttons[i].highlighted_ = false;
    }
}
