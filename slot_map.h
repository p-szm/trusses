//
//  slot_map.h
//  Trusses
//
//  Created by Patrick Szmucer on 23/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__slot_map__
#define __Trusses__slot_map__

#include <iostream>
#include <vector>

template <typename T>
class SlotMap
{
public:
    class iterator
    {
    public:
        void operator= (T* ptr) {pointer = ptr;}
        T& operator* () {return *pointer;}
        iterator operator++ (int dumy) {pointer ++; return *this;}
        bool operator!= (T* ptr) {if (pointer != ptr) return true; else return false;}
        T* operator->() { return pointer; }
    private:
        T* pointer;
    };
    
    T* begin();
    T* end();
    
    T& operator[] (unsigned int obj_id);// Returns the object of this id from the object container
    int add(T new_object); // Adds a new object to the container. TODO: Maybe pass this by reference?
    int remove(int obj_id); // Removes object of this id from the container
    void clear();
    void print(); // Prints all the objects together with the internal state of the slot map
    bool exists(unsigned int obj_id); // True if the objects exists in the container, false otherwise
private:
    std::vector<T> container; // Holds objects in unspecified order (not sorted by ids)
    std::vector<int> slots; // ith entry in this vector holds information about an object of id "i" in the container.
                            // If an object of id "i" doesn't exist anymore, it has slots[i] = -1
    std::vector<unsigned int> free_ids; // Holds free ids that can be used when adding a new object to the container
    unsigned int locate(int obj_id); // Looks up the id if an object in slots and returns its position in the container
};

// **** Implementation ****
//
// This has to go in the header file, because C++ doesn't like
// separating .h and .cpp files for template classes.

template <typename T>
T* SlotMap<T>::begin()
{
    if (container.size() == 0)
        return NULL;
    return &container[0];
}

template <typename T>
T* SlotMap<T>::end()
{
    if (container.size() == 0)
        return NULL;
    return &container.back()+1;
}

template <typename T>
int SlotMap<T>::add(T new_object)
{
    // This will either be a completely new ID
    // or one that existed before but is unused
    int new_id;
    
    if (!free_ids.empty()) // If there are some spare ids
    {
        new_id = free_ids.back();
        free_ids.pop_back();
        
        container.push_back(new_object);
        container.back().id_ = new_id;
        slots[new_id] = (int)container.size() - 1;
    }
    else // No spare ids. Create a new one
    {
        new_id = (int)container.size();
        container.push_back(new_object);
        container.back().id_ = new_id;
        
        slots.push_back(new_id);
    }
    
    return new_id;
}

template <typename T>
int SlotMap<T>::remove(int obj_id)
{
    // Check if this object exists. If not, return -1.
    bool exists = (obj_id < slots.size() && slots[obj_id] != -1) ? true : false;
    if (!exists)
    {
        std::cout << "Object " << obj_id << " doesn't exist" << std::endl;
        return 1;
    }
    
    // Swap the last object in the container with the object which will be removed.
    // This way we can just pop the object from the back, which is very fast.
    // container:  O O O O O R O O O O L  <- swap
    //             O O O O O L O O O O R  <- pop
    //             O O O O O L O O O O
    // Slot map has to be updatet to know the new position of the "last" element
    
    int swapped_obj_id = container.back().id_; // Remember the id of the last object
    int removed_object_pos = slots[obj_id]; // Position of the object in the container
    
    // Swap the object being destroyed with the last one
    T last_copy = container.back();
    container.back() = container[slots[obj_id]];
    container[removed_object_pos] = last_copy;
    
    // Pop the last object
    container.pop_back();
    
    // Update the slots
    slots[swapped_obj_id] = removed_object_pos;
    slots[obj_id] = -1; // The obj of this id doesn't exist anymore
    
    // Add the removed object's id to the free_ids list
    free_ids.push_back(obj_id);
    
    return 0;
}

template <typename T>
unsigned int SlotMap<T>::locate(int obj_id)
{
    return slots[obj_id];
}

template <typename T>
bool SlotMap<T>::exists(unsigned int obj_id)
{
    if (slots[obj_id] == -1)
        return false;
    return true;
}

// Responsibility of the user to ensure that this object exists in the container
template <typename T>
T& SlotMap<T>::operator[] (unsigned int obj_id)
{
    return container[locate(obj_id)];
}

template <typename T>
void SlotMap<T>::print()
{
    // Print the container
    std::cout << "Container: " << std::endl << " ";
    if (slots.size() == 0)
        std::cout << "none" << std::endl;
    else
    {
        for (int i = 0; i < container.size(); i++)
        {
            std::cout << container[i] << "(" << container[i].id_ << ")" << " ";
        }
    }
    std::cout << std::endl << std::endl;
    
    // Print the slots
    std::cout << "Slots: " << std::endl << " ";
    if (slots.size() == 0)
        std::cout << "none" << std::endl;
    else
    {
        for (int i = 0; i < slots.size(); i++)
        {
            std::cout << i << ":" << slots[i] << " ";
        }
    }
    std::cout << std::endl << std::endl;
    
    // Print free ids
    std::cout << "Free ids: " << std::endl << " ";
    if (free_ids.size() == 0)
        std::cout << "none" << std::endl;
    else
    {
        for (int i = 0; i < free_ids.size(); i++)
        {
            std::cout << free_ids[i] << " ";
        }
    }
    std::cout << std::endl << std::endl;
}

template <typename T>
void SlotMap<T>::clear()
{
    container.clear();
    slots.clear();
    free_ids.clear();
}

#endif /* defined(__Trusses__slot_map__) */
