// HashSet.hpp
//
// ICS 46 Spring 2021
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"


template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns false.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;

    // Sets up linked list nodes to store values into an array of linked lists.
    struct Node
    {
        ElementType element;
        Node* next;  // Only need a singly linked list; doubly linked is unncessary.
    };

    //Node* head;

    Node** nodeArray;  // Array of distinct linked lists (nodes).
    unsigned int cap;  // Capacity will initially be 10
};


// Constructs empty HashSet
template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}, nodeArray{new Node*[DEFAULT_CAPACITY]}, cap{DEFAULT_CAPACITY}
{
    Node* head = nullptr;

    for (int i = 0; i < cap; i++)
    {
        nodeArray[i] = head;
    }
}

// Deconstructor that deletes all nodes, then the array itself
template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    // Delete nodes
    for (int i = 0; i < cap; i++)
    {
        Node* currentNode = nodeArray[i];

        while (currentNode != nullptr)
        {
            Node* tempNode = currentNode;
            currentNode = currentNode->next;
            delete tempNode;
        }
    }

    // Delete array
    delete[] nodeArray;
}

// Copy Constructor
template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{s.hashFunction},
      nodeArray{new Node*[s.cap]}, cap{s.cap}
{
    Node* head = nullptr;

    // New array built with same capacity as other array
    for (int i = 0; i < s.cap; i++)
    {
        nodeArray[i] = head;
    }

    // Copy each node's element AND next pointer
    for (int j = 0; j < s.cap; j++)
    {
        Node* otherCurrentNode = s.nodeArray[j];
        // Node* thisCurrentNode = nodeArray[j];
        
        while (otherCurrentNode != nullptr)
        {
            nodeArray[j] = new Node{otherCurrentNode->element, nodeArray[j]};
            otherCurrentNode = otherCurrentNode->next;
        }
    }
}

// Move Constructor
// It should be enough to swap only the heads of each linked list, not each node
template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{s.hashFunction}, nodeArray{new Node*[s.cap]}, cap{s.cap}
{
    Node* head = nullptr;

    for (int i = 0; i < s.cap; i++)
    {
        nodeArray[i] = head;
    }

    // This nodeArray is now implemented to have Node* nullptrs in 
    // each of its indexes to be swapped
    std::swap(nodeArray, s.nodeArray);
}

// Assignment (copy) Constructor
template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if (this != &s)
    {
        // Delete nodes
        for (int i = 0; i < cap; i++)
        {
            Node* currentNode = nodeArray[i];

            while (currentNode != nullptr)
            {
                Node* tempNode = currentNode;
                currentNode = currentNode->next;
                delete tempNode;
            }
        }
        // Delete array
        delete[] nodeArray;

        // Allocate whole new array and perform a deep copy of each linked list
        nodeArray = new Node*[s.cap]; // need Node** ?
        cap = s.cap;

        Node* head = nullptr;

        // New array built with same capacity as other array
        for (int k = 0; k < s.cap; k++)
        {
            nodeArray[k] = head;
        }

        // Copy each node's element AND next pointer
        for (int j = 0; j < s.cap; j++)
        {
            Node* otherCurrentNode = s.nodeArray[j];
            // Node* thisCurrentNode = nodeArray[j];
            
            while (otherCurrentNode != nullptr)
            {
                nodeArray[j] = new Node{otherCurrentNode->element, nodeArray[j]};
                otherCurrentNode = otherCurrentNode->next;
            }
        }

        hashFunction = s.hashFunction;
    }

    return *this;
}

// Assignment Move Constructor
template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    std::swap(nodeArray, s.nodeArray);
    cap = s.cap;
    hashFunction = s.hashFunction;

    return *this;
}

// No need to implement this, only for grading purposes
template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

// If element already in index, break or return out of function
// Else, check first adding +1 to size would go over 0.8 and if so,
// then RESIZE array.  Afterwards, REHASH elements from previous array
// into new array.  Finally, "add" the element from this function into new
// array through hashing with new cap.
// New capacity formula: (current)capacity * 2 + 1
template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    // If not (less than 0.8) add the element into the array
    double sz = size();
    if ( ((sz + 1.0) / double(cap)) < 0.8 )
    {
        unsigned int keyLocation = (hashFunction(element) % cap);
        Node* currentNode = nodeArray[keyLocation];

        while (currentNode != nullptr)
        {
            if (currentNode->element == element)
            {
                return;
            }
            currentNode = currentNode->next;
        }
        nodeArray[keyLocation] = new Node{element, nodeArray[keyLocation]};
        // currentNode = new Node{element, currentNode};

    }

    // Array needs to be resized, rehashed, then add new element
    else
    {
        // create new capacity
        unsigned int newCap = (cap * 2) + 1;

        // Make whole new array with new capacity and fill with Node* heads
        Node** newNodeArray = new Node*[newCap];
        Node* head = nullptr;

        for (int i = 0; i < newCap; i++)
        {
            newNodeArray[i] = head;
        }

        // Rehash elements from old array into the new array
        for (int j = 0; j < cap; j++)
        {
            Node* currentNode = nodeArray[j];

            while (currentNode != nullptr)
            {
                // Insert nodes as heads each time
                unsigned int newIndex  = hashFunction(currentNode->element) % newCap;
                newNodeArray[newIndex] = new Node{currentNode->element, newNodeArray[newIndex]};

                currentNode = currentNode->next;
            }

        }

        // Delete the old array

        // Delete nodes
        for (int k = 0; k < cap; k++)
        {
            Node* currentNode = nodeArray[k];

            while (currentNode != nullptr)
            {
                Node* tempNode = currentNode;
                currentNode = currentNode->next;
                delete tempNode;
            }
        }

        // Delete array and update original nodeArray to become new array
        delete[] nodeArray;

        nodeArray = newNodeArray;


        // Add function parameter element to linked list of hashed index
        unsigned int hashedIndex  = hashFunction(element) % newCap;
        nodeArray[hashedIndex] = new Node{element, nodeArray[hashedIndex]};

        // Update this capacity to the new capacity
        cap = newCap;
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int index = hashFunction(element) % cap;
    
    // Constant time if good hash function
    Node* currentNode = nodeArray[index];
    while (currentNode != nullptr)
    {
        if (currentNode->element == element)
        {
            return true;
        }
        currentNode = currentNode->next;
    }

    // If element not found, return false
    return false;
}

// Returns the size (all the elements inside) of the Array.
// NOTE: Does *not* return the value of the member variable sz, but
// could be assigned to the member variable in constructors if needed.
template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    unsigned int count = 0;
    
    for (int i = 0; i < cap; i++)
    {
        // visit every index and then add up how many nodes are not nullptr
        Node* currentNode = nodeArray[i];

        while (currentNode != nullptr)
        {
            count++;
            currentNode = currentNode->next;
        }
    }

    return count;
}

// Returns numerical amount of objects in a specific index of the array
// If index is not within boundaries of array (greater than capacity) or
// no contents in index found, then return 0.
template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    if (index >= cap)
    {
        return 0;
    }
    else
    {
        unsigned int count = 0;
        
        // Visit the given index and then add up how many nodes are not nullptr
        Node* currentNode = nodeArray[index];

        while (currentNode != nullptr)
        {
            count++;
            currentNode = currentNode->next;
        }

        return count;
    }
}

// Return true if element is within selected index, else false
template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if (index >= cap)
    {
        return false;
    }
    else
    {
        // Visit the given index and then see if current index 
        // has the element in its linked list.
        Node* currentNode = nodeArray[index];

        while (currentNode != nullptr)
        {
            if (currentNode->element == element)
            {
                return true;
            }
            currentNode = currentNode->next;
        }

        // if element is not found within any nodes of the specific index, then false
        return false;
    }
}



#endif

