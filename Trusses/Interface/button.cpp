//
//  button.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 02/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "button.h"
#include "graphics.h"
#include "interface.h"
#include "save.h"
#include "wall_tool.h"
#include "selection_tool.h"
#include "obstacle_tool.h"
#include "renderer.h"
#include "window.h"

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
    change_state_ = true;
    
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
    ids = !ids;
}

void button_velocities_action(void)
{
    velocities = !velocities;
}

void button_accelerations_action(void)
{
    accelerations = !accelerations;
}

void button_lengths_action(void)
{
    lengths = !lengths;
}

void button_rel_extensions_action(void)
{
    extensions = !extensions;
}

void button_reset_action(void)
{
    reset_everything();
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

void button_draw_wall_action(void)
{
    Tool::set(current_tool, new WallTool);
}

void button_selection_action(void)
{
    Tool::set(current_tool, new SelectionTool);
}

void button_obstacle_action(void)
{
    Tool::set(current_tool, new ObstacleTool);
}

void create_buttons_editor()
{
    Button::create(60, 30, 1.0, 1.0, -50, -40, &button_save_action, "Save");
    Button::create(60, 30, 1.0, 1.0, -50, -85, &button_reset_action, "Reset");
    
    Button::create(60, 30, 1.0, 1.0, -50, -150, &button_lengths_action, "Lengths");
    Button::create(60, 30, 1.0, 1.0, -50, -195, &button_rel_extensions_action, "Rel ext");
    Button::create(60, 30, 1.0, 1.0, -50, -240, &button_draw_wall_action, "Wall");
    Button::create(60, 30, 1.0, 1.0, -50, -285, &button_selection_action, "Select");

    int plus_button_id = Button::create(60, 30, 1.0, 1.0, -50, -350, &button_scale_up_action, "     +");
    buttons[plus_button_id].change_state_ = false;
    int minus_button_id = Button::create(60, 30, 1.0, 1.0, -50, -395, &button_scale_down_action, "     -");
    buttons[minus_button_id].change_state_ = false;
    
    Button::create(60, 30, -1.0, 1.0, 50, -40, &button_obstacle_action, "Obstacle");
}

void create_buttons_simulation()
{
    Button::create(60, 30, 1.0, 1.0, -50, -40, &button_save_action, "Save");
    Button::create(60, 30, 1.0, 1.0, -50, -85, &button_reset_action, "Reset");
    
    Button::create(60, 30, 1.0, 1.0, -50, -150, &button_lengths_action, "Lengths");
    Button::create(60, 30, 1.0, 1.0, -50, -195, &button_rel_extensions_action, "Rel ext");
    
    int plus_button_id = Button::create(60, 30, 1.0, 1.0, -50, -350, &button_scale_up_action, "     +");
    buttons[plus_button_id].change_state_ = false;
    int minus_button_id = Button::create(60, 30, 1.0, 1.0, -50, -395, &button_scale_down_action, "     -");
    buttons[minus_button_id].change_state_ = false;
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
