#include <vector>
#include "deque.h"

/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

template <class T>
Deque<T>::Deque(){

//data = new vector<int>;
n1 = 0;
n2 = 0;

}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    data.push_back(newItem);
    n2++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    T value = data[n1];
    n1++;

    if (n2 - n1 <= n1) {   
        vector<T> temp;  

        for (int i= n1; i<n2; i++){     
            temp.push_back(data[i]);
        }
        
        n1 = 0;
        
        data = temp;
        
        n2 = data.size();  

    }
    return value; 
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    // //int value = data[n1+n2-1];
    
    // int value = data[n2-1];
    // if (n2+1 > n1) {

    // data.pop_back();
    // n2--;
    // //n2 = data.size();
    // return value;
    // }
    // //return T();

    ///

    
    T value = data[n2-1];
    n2--;
    data.pop_back();

    if (n2 - n1 <= n1) {   
        vector<T> temp;  

        for (int i= n1; i<n2; i++){     
            temp.push_back(data[i]);
        }
        
        n1 = 0;
        
        data = temp;
        
        n2 = data.size();  

    }
    return value; 

    

 }

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return data[n1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
     return data[n2-1];  
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return n2 == 0;
}
