#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdexcept>
#include "HelperFunctions.h"

/**
 * @file ArrayList.h
 * @brief Declaration and implementation of a dynamic array list for C++.
 * @author Arsen Niyetkaliyev
 * @date 2026-10-24
 */


/**
 * @class ArrayList
 * @brief Class representing a dynamic array list that can store elements of any type.
 * @tparam T The type of elements stored in the ArrayList.
 * 
 * A dynamic array list implementation that supports basic operations such as adding, removing, and accessing elements.
 */

template <typename T>
class ArrayList {
private:
    /** @brief Default capacity of the list */
    static const int DEFAULT_CAPACITY = 0;

    /** @brief Capacity of the list */
    int capacity;

    /** @brief Pointer to the array storing the elements */
    T* arr;

    /** @brief Current number of elements in the list */
    int arrSize;
    
public:
    /** 
	 * @brief Default constructor.
	 * @note Initializes the ArrayList with the default capacity.
	 */
    ArrayList() : ArrayList<T>(DEFAULT_CAPACITY) {}

    /**
	 * @brief Constructor with specified capacity.
	 * @param capacity The capacity of the ArrayList.
	 * @note Initializes the ArrayList with the specified capacity. If the capacity is not positive, throws invalid_argument.
	 */
    ArrayList(int capacity) {
        if(capacity < 0) {
            throw std::invalid_argument("Capacity must be non-negative.");
        }
        this->capacity = capacity;
        this->arr = new T[this->capacity];
        this->arrSize = 0;
    }

    /**
     * @brief Constructor that initializes the ArrayList with a given value.
     * @param size Number of elements.
     * @param val Value to fill the list with.
     */
    ArrayList(int size, const T& val) {
        if(size < 0) {
            throw std::invalid_argument("Capacity must be non-negative.");
        }
        this->capacity = size;
        this->arr = new T[this->capacity];
        this->arrSize = size;

        for(int i = 0; i < size; i++) {
            this->arr[i] = val;
        }
    }

    /** 
	 * @brief Copy constructor.
	 * @param orig The ArrayList object to deep copy from.
	 */
    ArrayList(const ArrayList& orig) {
        this->capacity = orig.capacity;
        this->arr = new T[this->capacity];
        this->arrSize = orig.arrSize;
        for(int i = 0; i < this->arrSize; i++) {
            this->arr[i] = orig.arr[i];
        }
    }

    /** 
	 * @brief Assignment operator.
	 * @param orig The ArrayList object to deep copy from.
	 * @return Reference to current ArrayList object.
	 */
    ArrayList& operator=(const ArrayList& orig) {
        if(this == &orig) {
            return *this;
        }

        ArrayList<T> temp(orig);
        helperSwap(this->arr, temp.arr);
        helperSwap(this->capacity, temp.capacity);
        helperSwap(this->arrSize, temp.arrSize);

        // temp will be destroyed here, releasing the old resources of this object
        return *this;
    }

    /** 
	 * @brief Destructor.
	 * @note Releases the dynamically allocated memory for the array.
	 */
    ~ArrayList() {
        delete[] this->arr;
    }

    /**
     * @brief Access operator to get the element at the specified index.
     * @param index The index of the element to access.
     * @return Reference to the element at the specified index.
     */
    T& operator[](int index) {
        if(index < 0 || index >= this->arrSize) {
            throw std::out_of_range("Index out of range.");
        }
        return this->arr[index];
    }

    /**
     * @brief Const access operator to get the element at the specified index.
     * @param index The index of the element to access.
     * @return Const reference to the element at the specified index.
     * @note This operator is used when the ArrayList object is const.
     */
    const T& operator[](int index) const {
        if(index < 0 || index >= this->arrSize) {
            throw std::out_of_range("Index out of range.");
        }
        return this->arr[index];
    }

    /**
     * @brief Adds an element to the end of the list.
     * @param val The value to be added to the list.
     * @note If the current size of the list equals its capacity, the capacity is doubled before adding the new element.
     */
    void pushBack(const T& val) {
        if(this->arrSize == this->capacity) {
            int newCapacity = this->capacity*2;
            if(newCapacity == 0) {
                newCapacity = 1;
            }
            ArrayList<T> temp(newCapacity);
            temp.arrSize = this->arrSize;
            for(int i = 0; i < this->arrSize; i++) {
                temp.arr[i] = this->arr[i];
            }
            helperSwap(this->arr, temp.arr);
            helperSwap(this->capacity, temp.capacity);
        }
        this->arr[this->arrSize] = val;
        this->arrSize++;
    }

    /**
     * @brief Removes the last element from the list.
     * @note If the list is empty, an out_of_range exception is thrown.
     */
    void popBack() {
        if(this->arrSize == 0) {
            throw std::out_of_range("Array is empty.");
        }
        this->arrSize--;
        // Reset the last element to default value after popping.
        // This ensures that the memory previously occupied by the last element is properly reset.
        this->arr[this->arrSize] = T(); 
    }

    /**
     * @brief Clears the list, removing all elements.
     * @note The capacity of the list remains unchanged after clearing.
     */
    void clear() {
        ArrayList<T> temp;
        helperSwap(this->arr, temp.arr);
        helperSwap(this->capacity, temp.capacity);
        helperSwap(this->arrSize, temp.arrSize);
    }

    /**
     * @brief Returns the number of elements in the list.
     * @return The number of elements in the list.
     */
    int getSize() const {
        return this->arrSize;
    }

    /**
     * @brief Checks if the list is empty.
     * @return True if the list is empty, false otherwise.
     */
    bool isEmpty() const {
        return this->arrSize == 0;
    }
};

#endif
