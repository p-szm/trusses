//
//  button.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 02/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "button.h"
#include "graphics.h"

int buttons_number = 0;
std::vector<Button> buttons {};

Button::Button(double w, double h, double x_sh, double y_sh, void (*a)(void))
{
    width_ = w;
    height_ = h;
    x_shift_ = x_sh;
    y_shift_ = y_sh;
    action = a;
    
    id = buttons_number;
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

void button0_action(void)
{
    ids = !ids;
}

void button1_action(void)
{
    velocities = !velocities;
}

void create_buttons()
{
    buttons.push_back(Button(50, 30, -20, -20, &button0_action));
    buttons.push_back(Button(50, 30, -20, -70, &button1_action));
}