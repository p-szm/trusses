//
//  fixed_size.h
//  Trusses
//
//  Created by Patrick Szmucer on 23/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include <vector>
#include <stdexcept>

template <typename T>
class FixedSizeContainer
{
public:
    T* begin();
    const T* begin() const;
    T* end();
    const T* end() const;
    
    void add(T obj);
    void print() const;
    const T& last_element() const;
    unsigned int size() const;
    const T& get(unsigned int i) const;
    void clear();
    FixedSizeContainer(unsigned int n): max_size(n) {current_pos = 0;}
    //private:
    unsigned int max_size;
    std::vector<T> container;
    unsigned int current_pos;
};

template <typename T>
void FixedSizeContainer<T>::add(T obj)
{
    if (container.size() < max_size)
        container.push_back(obj);
    else if (container.size() == max_size)
    {
        container[current_pos] = obj;
    }
    current_pos++;
    if (current_pos == max_size)
        current_pos = 0;
}

template <typename T>
void FixedSizeContainer<T>::print() const
{
    for (int i = 0; i < container.size(); i++)
        std::cout << container[i] << " ";
    std::cout << std::endl;
}

template <typename T>
const T& FixedSizeContainer<T>::last_element() const
{
    if (size() == 0)
        throw std::out_of_range("there are no elements in the container");
    if (current_pos == 0)
        return container.back();
    return container[current_pos - 1];
}

template <typename T>
unsigned int FixedSizeContainer<T>::size() const
{
    return (unsigned int)container.size();
}

template <typename T>
const T& FixedSizeContainer<T>::get(unsigned int i) const
{
    if (i >= size())
        throw std::out_of_range("index out of range");
    
    size_t index;
    
    // If the container was already wrapped
    if (size() == max_size)
        index = (current_pos + i) % max_size;
    else
        index = i;
    
    return container.at(index);
}

template <typename T>
void FixedSizeContainer<T>::clear()
{
    container.clear();
    current_pos = 0;
}
