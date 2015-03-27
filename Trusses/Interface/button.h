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
#include "math.h"

class Renderer;

class Button
{
    friend class Renderer;
public:
    int id_;
    std::string text_;
    bool highlighted_; // Highlighted when the mouse is hovered over it
    bool active_; // State of the button
    static int create(double w, double h, double pos_x, double pos_y, int off_x, int off_y, void (*a)(void), std::string t);
    bool is_hit(double x, double y);// Checks if x and y are inside the button's limits. x and y are in gl coords
    void execute_action(); // Executes the action and changes the button's state
    void draw(const Renderer& rend) const;
    friend void create_buttons_editor(); // It needs to access the private change_state_ variable
    friend void create_buttons_simulation();
    
private:
    void (*action)(void); // This will be executed when the button is clicked
    double width_; // In px
    double height_; // In px
    bool change_state_; // If false this button can be clicked multiple times without changing its state
    
    Vector2d position; // In gl coordinates, anchored to the button's centre
    Vector2d offset; // In pixels
    
    Button(double w, double h, double pos_x, double pos_y, int off_x, int off_y, void (*a)(void), std::string t);
};

extern std::vector<Button> buttons;

void create_buttons_editor();
void create_buttons_simulation();
void highlight_buttons(double x, double y);

#endif /* defined(__Trusses__button__) */
