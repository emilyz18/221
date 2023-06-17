#include "chain.h"
#include "chain_given.cpp"
#include <cmath>
#include <iostream>

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain()
{


  clear();


}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block &ndata)
{
  Node *newNode = new Node(ndata);

        
  Node*lastPointer = walk(head_, length_);

  newNode -> next = head_;
  lastPointer->next = newNode;
  length_++;
}




/**
 * Swaps the two nodes at the indexes "node1" and "node2".
 * The indexes of the nodes are 1 based.
 * assumes i and j are valid (in {1,length_} inclusive)
 * 
 */
void Chain::swap(int i, int j)
{
/* YOUR CODE HERE */

Node * firstSwapNode  = walk(head_, i-1);
Node * secondSwapNode = walk(head_,j-1);

Node*temp = firstSwapNode->next;
Node*temp2 = firstSwapNode->next->next;

if (i == j-1) {


firstSwapNode->next = secondSwapNode->next;
 temp->next = secondSwapNode->next->next;

// secondSwapNode->next->next = temp2;
// secondSwapNode->next = temp;

firstSwapNode->next->next = secondSwapNode;
} else {
firstSwapNode->next = secondSwapNode->next;
 temp->next = secondSwapNode->next->next;

secondSwapNode->next->next = temp2;
secondSwapNode->next = temp;
}


}

/**
 * Reverses the chain
 */
void Chain::reverse()
{

// Node *tempLastElement;
// int length = length_;
// int secondLength = length_;

// Node *p;
// p = head_;
// while (length > 0) {
//   p = p->next;
//   length--;
// }

// tempLastElement = p;

// for (int i = 1; i < secondLength; i++) {
//   p->next = walk(head_, secondLength - i);
//   p = p->next;
// }

// p->next = head_;


// head_->next = tempLastElement;


int moveForrward = 1;
int moveBackward = length_;

while (moveForrward < moveBackward) {
  swap(moveForrward,moveBackward);
  moveForrward++;
  moveBackward--;
}

}


/*
* Modifies the current chain by "rotating" every k nodes by one position.
* In every k node sub-chain, remove the first node, and place it in the last 
* position of the sub-chain. If the last sub-chain has length less than k,
* then don't change it at all. 
* Some examples with the chain a b c d e:
*   k = 1: a b c d e
*   k = 2: b a d c e
*   k = 3: b c a d e
*   k = 4: b c d a e
*/
void Chain::rotate(int k)
{

int temp = k;
int beginning = 1;
for (int b = 0; b < length_-1; b = b+k) {
    for (int a = 2+b; a <= temp; a++) {
      swap(beginning,a);
      beginning = beginning + 1;
    }

    beginning = beginning + 1;
    temp = temp + k;
}
}

/**
 * Destroys ALL dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear()
{

  length_ = 0;
  Node *nextNode = head_->next;

  while(nextNode != head_){
    Node *temp = nextNode;

    nextNode=temp->next;

    delete temp;
    temp=NULL;
  }

  delete head_;
  head_ = NULL;

  }



/* makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other)
{

  width_ = other.width_;
  height_ = other.height_;
  length_ = other.length_;

  Node* otherNode = other.head_->next;
   head_ = new Node();
   head_->next = head_;
   Node* thisNode = head_;
   

  for (int i = 0; i< other.size(); i++) {
    Node* newNode = new Node(otherNode->data);
    newNode->next = head_;
    thisNode->next = newNode;

     thisNode = thisNode->next;
     otherNode = otherNode->next;
  
  }

}

  
  

