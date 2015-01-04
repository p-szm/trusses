//
//  button.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 02/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "button.h"
#include "graphics.h"
#include "save.h"
#include "physics.h"

int buttons_number = 0;
std::vector<Button> buttons {};

Button::Button(double w, double h, double x_sh, double y_sh, void (*a)(void), std::string t)
{
    width_ = w;
    height_ = h;
    x_shift_ = x_sh;
    y_shift_ = y_sh;
    action = a;
    highlighted_ = false;
    text_ = t;
    active_ = false;
    change_state_ = true;
    
    id_ = buttons_number;
    buttons_number ++;
    
    update_position(); // Updates the button's centre
}

bool Button::is_hit(double x, double y)
{
    double left = centre_.x - width_ / 2.0;
    double right = centre_.x + width_ / 2.0;
    double top = centre_.y + height_ / 2.0;
    double bottom = centre_.y - height_ / 2.0;
    
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

void Button::update_position()
{
    // Used when the window is resized
    
    double half_width = width_ / 2.0;
    double half_height = height_ / 2.0;
    
    if (x_shift_ >= 0)
        centre_.x = -window_width / 2.0 + half_width + x_shift_;
    else
        centre_.x = window_width / 2.0 - half_width + x_shift_;
    
    if (y_shift_ >= 0)
        centre_.y = -window_height / 2.0 + half_height + y_shift_;
    else
        centre_.y = window_height / 2.0 - half_height + y_shift_;
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

void button_coords_action(void)
{
    coords = !coords;
}

void button_snap_action(void)
{
    snap = !snap;
}

void button_reset_action(void)
{
    reset();
}

void button_save_action(void)
{
    std::string path = "/Users/patrick/Desktop/save-";
    path += date_str() + '-' + time_str();
    save(path);
}

void button_gravity_action(void)
{
    gravity = !gravity;
}

void button_scale_up_action(void)
{
    scale *= 1.1;
}

void button_scale_down_action(void)
{
    scale /= 1.1;
}

void button_draw_wall_action(void)
{
    drawing_wall = true;
}

void create_buttons()
{
    buttons.push_back(Button(60, 30, -20, -20, &button_ids_action, "Ids"));
    buttons.push_back(Button(60, 30, -20, -70, &button_velocities_action, "Vels"));
    buttons.push_back(Button(60, 30, -20, -120, &button_accelerations_action, "Accels"));
    buttons.push_back(Button(60, 30, -20, -170, &button_lengths_action, "Lengths"));
    buttons.push_back(Button(60, 30, -20, -220, &button_rel_extensions_action, "Rel ext"));
    buttons.push_back(Button(60, 30, -20, -270, &button_coords_action, "Coords"));
    buttons.push_back(Button(60, 30, -20, -320, &button_snap_action, "Snap"));
    buttons[6].active_ = true;
    buttons.push_back(Button(60, 30, -20, -420, &button_reset_action, "Reset"));
    buttons[7].change_state_ = false;
    buttons.push_back(Button(60, 30, -20, -470, &button_save_action, "Save"));
    buttons[8].change_state_ = false;
    buttons.push_back(Button(60, 30, 23, -30, &button_gravity_action, "Gravity"));
    buttons.push_back(Button(60, 30, -20, -570, &button_scale_up_action, "     +"));
    buttons[10].change_state_ = false;
    buttons.push_back(Button(60, 30, -20, -620, &button_scale_down_action, "     -"));
    buttons[11].change_state_ = false;
    buttons.push_back(Button(60, 30, -20, -670, &button_draw_wall_action, "Wall")); // 12
    
}