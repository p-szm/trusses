//
//  window.cpp
//  Trusses
//
//  Created by Patrick on 08/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "window.h"
#include "interface.h"
#include "various_math.h"

Window window(1000, 700, 50);

Window::Window(double w, double h, double s): width(w), height(h), scale(s)
{
    centre = Vector2d(0.0, 0.0);
    scroll_speed = 250;
    full_screen = false;
    original_scale = scale;
    zoom_speed = 0.25;
    
    scale_min = 0.1;
    scale_max = 1000;
    target_scale = scale;
}

void Window::update(const Arrows& arr, const double dt)
{
    // Move the centre
    if (arr.left)
        centre.x -= px_to_m(scroll_speed) * dt;
    if (arr.right)
        centre.x += px_to_m(scroll_speed) * dt;
    if (arr.up)
        centre.y += px_to_m(scroll_speed) * dt;
    if (arr.down)
        centre.y -= px_to_m(scroll_speed) * dt;
    
    // Zoom
    double err = abs_d(scale - target_scale) / target_scale;
    if (err < 0.01)
        scale = target_scale;
    
    scale += zoom_speed * (target_scale - scale);
}

double Window::left() const
{
    return -window.width/(2.0*window.scale) + window.centre.x;
}

double Window::right() const
{
    return window.width/(2.0*window.scale) + window.centre.x;
}

double Window::bottom() const
{
    return -window.height/(2.0*window.scale) + window.centre.y;
}

double Window::top() const
{
    return window.height/(2.0*window.scale) + window.centre.y;
}

void Window::reset()
{
    scale = original_scale;
    centre = Vector2d(0.0, 0.0);
}

double Window::get_scale() const
{
    return scale;
}

void Window::set_scale(double s)
{
    if (s >= scale_min && s <= scale_max)
        target_scale = s;
}

void Window::scale_by(double d)
{
    double new_scale = d * scale;
    if (new_scale >= scale_min && new_scale <= scale_max)
        set_scale(new_scale);
}
