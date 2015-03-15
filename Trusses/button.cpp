//
//  button.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 02/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "button.h"
#include "graphics.h"
#include "interface.h"
#include "save.h"
#include "physics.h"

int buttons_number = 0;
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
    
    id_ = buttons_number;
    buttons_number ++;
}

int Button::create(double w, double h, double pos_x, double pos_y, int off_x, int off_y, void (*a)(void), std::string t)
{
    Button b = Button(w, h, pos_x, pos_y, off_x, off_y, a, t);
    buttons.push_back(b);
    return b.id_;
}

bool Button::is_hit(double x, double y)
{
    double left = position.x + px_to_gl_x(offset.x - width_ / 2.0);
    double right = position.x + px_to_gl_x(offset.x + width_ / 2.0);
    double top = position.y + px_to_gl_y(offset.y + height_ / 2.0);
    double bottom = position.y + px_to_gl_y(offset.y - height_ / 2.0);
    
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
    world.scale *= 1.1;
}

void button_scale_down_action(void)
{
    world.scale /= 1.1;
}

void button_draw_wall_action(void)
{
    if (!drawing_wall)
        start_drawing_wall();
    else
        stop_drawing_wall();
}

void create_buttons()
{
    Button::create(60, 30, 1.0, 1.0, -50, -40, &button_ids_action, "Ids");
    Button::create(60, 30, 1.0, 1.0, -50, -90, &button_velocities_action, "Vels");
    Button::create(60, 30, 1.0, 1.0, -50, -140, &button_lengths_action, "Lengths");
    Button::create(60, 30, 1.0, 1.0, -50, -190, &button_rel_extensions_action, "Rel ext");
    Button::create(60, 30, 1.0, 1.0, -50, -240, &button_reset_action, "Reset");
    Button::create(60, 30, 1.0, 1.0, -50, -290, &button_save_action, "Save");
    int plus_button_id = Button::create(60, 30, 1.0, 1.0, -50, -340, &button_scale_up_action, "     +");
    buttons[plus_button_id].change_state_ = false;
    int minus_button_id = Button::create(60, 30, 1.0, 1.0, -50, -390, &button_scale_down_action, "     -");
    buttons[minus_button_id].change_state_ = false;
    Button::create(60, 30, 1.0, 1.0, -50, -440, &button_draw_wall_action, "Wall");
    
}

void highlight_buttons(double x, double y)
{
    bool button_hit = false;
    for (int i = 0; i < buttons_number && button_hit == false; i++)
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
