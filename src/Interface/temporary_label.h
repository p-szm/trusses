//
//  temporary_label.h
//  Trusses
//
//  Created by Patrick Szmucer on 31/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__temporary_label__
#define __Trusses__temporary_label__

#include <iostream>
#include "vector2d.h"
#include "slot_map.h"

#define MODE_LABEL_TIME 4000000
#define TOOL_LABEL_TIME 3000000
#define WARNING_LABEL_TIME 3000000
#define INFO_LABEL_TIME 3000000
#define BOTTOM_MARGIN 20
#define TOP_MARGIN 25

class Renderer;

// Displays text at given position for given amount of time.
// The text starts fading away after 70% of its lifetime.
class TempLabel
{
    friend class Renderer;
public:
    int id_;
    
    // In microseconds
    unsigned long long int max_time;
    
    // Current time
    unsigned long long int time;
    
    // Anchored to the middle
    static int create(std::string str, double pos_x, double pos_y,
                      int off_x, int off_y, double t, bool centre_l = true);
    
    // Updates the label, returns 1 if it should be destroyed,
    // 0 otherwise. This method shoudld be called every frame.
    int update();
    
    void draw(const Renderer& rend) const;

    // Removes the label with given id
    static int destroy(int obj_id);
    
protected:
    // Text to be displayed
    std::string text;
    
    // In world coords. The anchor point is defined
    // by the "centre" variable.
    Vector2d position;
    
    // Offset in pixel from the oiginal position
    Vector2d offset;
    
    // Returns the current opacity (1.0 means full opacity)
    float alpha() const;
    
    // True if the label is anchored to the centre, false
    // if it's anchored to the bottom left corner.
    bool centre;
private:
    // Constructor is private. TempLabel::create should be used instead.
    TempLabel(std::string str, double pos_x, double pos_y, int off_x,
              int off_y, double t, bool centre_l = true);
};

// Updates labels and removes the ones that expired
void update_labels();

// Creates a label with given text in the bottom left corner
void issue_label(std::string text, unsigned long long int text_time);

extern SlotMap<TempLabel> temp_labels;

#endif /* defined(__Trusses__temporary_label__) */
