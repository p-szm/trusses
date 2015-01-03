//
//  button.h
//  Oscillator
//
//  Created by Patrick Szmucer on 02/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Oscillator__button__
#define __Oscillator__button__

#include <iostream>
#include <vector>

#include "math.h" // for Vector2d

class Button
{
public:
    friend void create_buttons();
    
    // All in pixels relative to the top left corner
    Button(double w, double h, double x_sh, double y_sh, void (*a)(void), std::string t);
    friend void draw();
    bool is_hit(double x, double y);
    void execute_action();
    int id_;
    std::string text_;
    void update_position();
    friend void draw_button(const Button& rect);
    bool highlighted_;
    bool active_;
private:
    void (*action)(void);
    double x_shift_;
    double y_shift_;
    Vector2d centre_;
    double width_;
    double height_;
    bool change_state_;
};

void button0_action(void);
void button1_action(void);

extern std::vector<Button> buttons;
extern int buttons_number;

void create_buttons();

#endif /* defined(__Oscillator__button__) */