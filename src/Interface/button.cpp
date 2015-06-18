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
#include "split_tool.h"
#include "bars_tool.h"
#include "measure_tool.h"
#include "delete_tool.h"
#include "renderer.h"
#include "window.h"
#include "game.h"
#include "settings.h"

std::vector<Button> buttons;

Button::Button(double w, double h, double pos_x, double pos_y,
               int off_x, int off_y, void (*a)(),
               bool (*ac)(), std::string t)
{
    width_ = w;
    height_ = h;
    position = Vector2d(pos_x, pos_y);
    offset = Vector2d(off_x, off_y);
    text_ = t;
    
    action = a;
    active_on = ac;
    highlighted_ = false;
    active_ = false;
    
    id_ = (int)buttons.size();
}

int Button::create(double w, double h, double pos_x, double pos_y,
           int off_x, int off_y, void (*a)(),
           bool (*ac)(), std::string t)
{
    Button b = Button(w, h, pos_x, pos_y, off_x, off_y, a, ac, t);
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

bool Button::is_highlighted()
{
    return highlighted_;
}

void Button::draw(const Renderer& rend) const
{
    rend.render(*this);
}

void Button::execute_action()
{
    action();
}

// ids
void action_ids()
{
    settings.toggle(IDS);
}

bool active_ids()
{
    return settings.get(IDS);
}

// Lengths
void action_lengths()
{
    settings.toggle(LENGTHS);
}

bool active_lengths()
{
    return settings.get(LENGTHS);
}

// Extensions
void action_extensions()
{
    settings.toggle(EXTENSIONS);
}

bool active_extensions()
{
    return settings.get(EXTENSIONS);
}

// Reset
void action_reset()
{
    game.reset();
}

// Save
void action_save()
{
    std::string path = "/Users/patrick/Desktop/save-";
    path += date_str() + '-' + time_str();
    save(path);
}

// Scale
void action_scale_up()
{
    window.scale *= 1.1;
}

void action_scale_down()
{
    window.scale /= 1.1;
}

// Triangulation
void action_triangulation()
{
    settings.toggle(TRIANGULATION);
}

bool active_triangulation()
{
    return settings.get(TRIANGULATION);
}

// Bounding boxes
void action_bboxes()
{
    settings.toggle(BOUNDING_BOXES);
}

bool active_bboxes()
{
    return settings.get(BOUNDING_BOXES);
}

// Simulate
void action_simulate()
{
    game.enter_simulation();
}

// Edit
void action_edit()
{
    game.enter_editor();
}

// Selection tool
void action_selection_tool()
{
    Tool::set(current_tool, new SelectionTool);
}

bool active_selection_tool()
{
    return (current_tool->get_name() == SELECTION);
}

// Obstacle tool
void action_obstacle_tool()
{
    Tool::set(current_tool, new ObstacleTool);
}

bool active_obstacle_tool()
{
    return (current_tool->get_name() == OBSTACLE);
}

// Bars
void action_bars_tool()
{
    Tool::set(current_tool, new BarsTool);
}

bool active_bars_tool()
{
    return (current_tool->get_name() == BARS);
}

// Trace
void action_trace_tool()
{
    Tool::set(current_tool, new TraceTool);
}

bool active_trace_tool()
{
    return (current_tool->get_name() == TRACE);
}

// Split
void action_split_tool()
{
    Tool::set(current_tool, new SplitTool);
}

bool active_split_tool()
{
    return (current_tool->get_name() == SPLIT);
}

// Measure
void action_measure_tool()
{
    Tool::set(current_tool, new MeasureTool);
}

bool active_measure_tool()
{
    return (current_tool->get_name() == MEASURE);
}

// Delete
void action_delete_tool()
{
    Tool::set(current_tool, new DeleteTool);
}

bool active_delete_tool()
{
    return (current_tool->get_name() == DELETE);
}

void create_buttons_editor()
{
    const int tmargin = 40;
    const int lmargin = 50;
    const int w = 60;
    const int h = 30;
    const int dy = 40;
    
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin, &action_save, NULL, "Save");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-dy, &action_reset, NULL, "Reset");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-2.5*dy, &action_ids, &active_ids, "Ids");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-3.5*dy, &action_lengths, &active_lengths, "Lengths");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-4.5*dy, &action_extensions, &active_extensions, "Rel ext");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-5.5*dy, &action_triangulation, &active_triangulation, "Triangles");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-6.5*dy, &action_bboxes, &active_bboxes, "B-boxes");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-8*dy, &action_scale_up, NULL, "     +");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-9*dy, &action_scale_down, NULL, "     -");
    
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-10.5*dy, &action_simulate, NULL, "Simulate");
    
    Button::create(w, h, 1.0, 1.0, -lmargin, -tmargin, &action_bars_tool, &active_bars_tool, "Draw");
    Button::create(w, h, 1.0, 1.0, -lmargin, -tmargin-dy, &action_selection_tool, &active_selection_tool, "Select");
    Button::create(w, h, 1.0, 1.0, -lmargin, -tmargin-2*dy, &action_obstacle_tool, &active_obstacle_tool, "Obstacle");
    Button::create(w, h, 1.0, 1.0, -lmargin, -tmargin-3*dy, &action_trace_tool, &active_trace_tool, "Trace");
    Button::create(w, h, 1.0, 1.0, -lmargin, -tmargin-4*dy, &action_split_tool, &active_split_tool, "Split");
    Button::create(w, h, 1.0, 1.0, -lmargin, -tmargin-5*dy, &action_measure_tool, &active_measure_tool, "Measure");
    Button::create(w, h, 1.0, 1.0, -lmargin, -tmargin-6*dy, &action_delete_tool, &active_delete_tool, "Delete");
    
    refresh_buttons();
}

void create_buttons_simulation()
{
    const int tmargin = 40;
    const int lmargin = 50;
    const int w = 60;
    const int h = 30;
    const int dy = 40;
    
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin, &action_save, NULL, "Save");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-dy, &action_reset, NULL, "Reset");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-2.5*dy, &action_ids, &active_ids, "Ids");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-3.5*dy, &action_lengths, &active_lengths, "Lengths");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-4.5*dy, &action_extensions, &active_extensions, "Rel ext");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-5.5*dy, &action_triangulation, &active_triangulation, "Triangles");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-6.5*dy, &action_bboxes, &active_bboxes, "B-boxes");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-8*dy, &action_scale_up, NULL, "     +");
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-9*dy, &action_scale_down, NULL, "     -");
    
    Button::create(w, h, -1.0, 1.0, lmargin, -tmargin-10.5*dy, &action_edit, NULL, "Edit");
    
    refresh_buttons();
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
        {
            buttons[i].highlighted_ = false;
        }
    }
}

void refresh_buttons()
{
    for (int i = 0; i < buttons.size(); i++)
    {
        if (buttons[i].active_on)
        {
            buttons[i].active_ = buttons[i].active_on();
        }
    }
}
