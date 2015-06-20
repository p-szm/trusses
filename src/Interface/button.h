//
//  button.h
//  Trusses
//
//  Created by Patrick Szmucer on 02/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__button__
#define __Trusses__button__

#include <vector>
#include "vector2d.h"
#include "tool.h"

class Renderer;

class Button
{
    friend class Renderer;
public:
    int id_;
    std::string text_;
    
    // Should be called when events that can change the button state
    // happen
    friend void refresh_buttons();
    
    // Highlights buttons if the mouse is hovered over them
    friend void highlight_buttons(double x, double y);
    
    static int create(double w, double h, double pos_x, double pos_y,
                      int off_x, int off_y, void (*a)(),
                      bool (*ac)(), std::string t);
    
    // Checks if x and y are inside the button's limits.
    // x and y are in gl coords
    bool is_hit(double x, double y);
    
    // True if the button is highlighted. Equivalent to the is_hit function,
    // but much faster (but is_hit must be called first each frame).
    bool is_highlighted();
    
    // Executes the button's action
    void execute_action();
    
    void draw(const Renderer& rend) const;
    
private:
    // This will be executed when the button is clicked
    void (*action)();
    
    // True if the button is in the active state
    bool (*active_on)();
    
    // Highlighted when the mouse is hovered over it
    bool highlighted_;
    
    // State of the button
    bool active_;
    
    // In px
    double width_;
    double height_;
    
    // In gl coordinates, anchored to the button's centre
    Vector2d position;
    
    // In pixels
    Vector2d offset;
    
    Button(double w, double h, double pos_x, double pos_y,
           int off_x, int off_y, void (*a)(),
           bool (*ac)(), std::string t);
};

extern std::vector<Button> buttons;

void create_buttons_editor();
void create_buttons_simulation();
void highlight_buttons(double x, double y);
void refresh_buttons();

#endif /* defined(__Trusses__button__) */
