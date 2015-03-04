#include <vector>

template <typename T>
class FixedSizeContainer
{
public:
    void add(T obj);
    void print() const;
    const T& last_element() const;
    unsigned int size() const;
    const T& get(unsigned int i) const;
    void clear();
    FixedSizeContainer(unsigned int n): max_size(n) {current_pos = 0;}
private:
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
    return container.at(i);
}

template <typename T>
void FixedSizeContainer<T>::clear()
{
    container.clear();
    current_pos = 0;
}