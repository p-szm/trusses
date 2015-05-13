# Trusses

## Overview  
This application allows you to draw and simulate truss-like structures. It was created using OpenGL with GLUT with no other external libraries. Tested on OS X and Linux.

## Build   
```
mkdir build && cd build
cmake ..
make
```

## Controls  
Left-click to create joints which can then be connected with bars. Press "Simulate" to start the simulation, and "Edit" to pause it.
Additional tools are available by clicking on different buttons.

## Command line  
Command line can be accessed by pressing the Tab key. Example commands available:  
```load tower.tr```  
```save tower_modified.tr```  
```gravity on```  
```gravity off```  

## File format  
Example file format:
```
2015-5-13 01-01-20

p0 0.1 0.1
p1 0.1 3.4
f2 -3.0 0.4

b0 0 1 0.0
b1 1 2 0.0
b2 2 0 0.0

o0 0.0 0.0 1.0 0.0 1.0 1.0 0.0 1.0
```
Joint: ```p<unique joint id> <x_pos> <y_pos>```  
Fixed joint: ```p<unique joint id> <x_pos> <y_pos>```  
Bar: ```b<unique bar id> <particle 1> <particle 2>```  
Obstacle: ```o<unique obstacle id> <x1> <y1> <x2> <y2> ...```    