#include "tree.hpp"
#include <iostream>
#include <string>
#include <string_view>

// Constructor
Tree::Tree(int l, int t) {
  // set appropriate fields to tree node
  txt = "";
  len = l;
  tok = t;
  left = nullptr; // left and right ptr is alway null to start since no children
                  // to start with
  rpn = "";       // rpn of the tree
  right = nullptr;
  parent = 1; // Used for depth first tree traversal
}

// Destroy the node by using code similar to post order traversal
void Tree::destroy_node(Tree *t) {
  if (t->left == nullptr && t->right == nullptr) {
    // std::cout<<"Deleting: "<<t->get_txt()<<"\n";
    delete t;
    t = nullptr;
    return;
  }
  destroy_node(t->left);  // Go through left side of tree
  destroy_node(t->right); // Go through right side of tree
  // std::cout<<"Deleting: "<<t->get_txt()<<"\n";
  delete t;
  t = nullptr;
}

// Returns text of current node
std::string_view Tree::get_txt() const { 
  return txt; 
}

// Returns token type of current node in string form
std::string_view Tree::get_tok() const {
  std::string_view temp = std::to_string(tok);
  return temp;
}

// Returns the Reverse Polish Notation of the tree
void Tree::get_rpn() {
  if (rpn == "") {
    std::string temp =""; // Start with an empty string used for post order traversal
    traverse_post_order(this); // Go through post order traversal and return the string of
    // the path of post order traversal
    rpn.pop_back(); // remove the end whitespace
    // std::cout<<rpn<<"\n";
  }
}

// Traverse Tree in post order form. Used to help with the get_rpn() function
// Input: Takes in a tree pointer to a node and rpn string at current state
// Output: Returns the complete rpn string
void Tree::traverse_post_order(const Tree *t) {
  if (t->left == nullptr &&t->right == nullptr) { // Checks if reached a leaf node
    // std::cout<<t->get_txt()<<"\n";
    rpn = rpn + t->txt; // add the token text to the rpn string
    rpn = rpn + " ";    // Add space to end of new rpn string
    return;
  }

  traverse_post_order(t->left);  // traverse through left side of tree
  traverse_post_order(t->right); // traverse through right side of tree
  rpn = rpn + t->txt;            // add token to rpn string
  rpn = rpn + " ";               // append space to end of string
  // std::cout<<t->get_txt()<<"\n";
  return;
}

// Traverse the tree using depth first preoder traveral meaning visiting the
// left and right child of each parent Input: takes in a function pointer to a
// void function with a tree node for the parameter. Output:none
void Tree::traverse_depth_first(void (*fun)(const Tree *)) {

  if (left == nullptr && right == nullptr) { // Checks if end is reached
    return;
  }
  if (parent == 0) { // Checks if this is the root
    // std::cout<<"Top: ";
    (*fun)(this);
  }
  // std::cout<<"Left: ";
  (*fun)(left); // Start with the left child
  left->traverse_depth_first((*fun));
  // std::cout<<"Right: ";
  (*fun)(right); // Now continue to the right children
  right->traverse_depth_first((*fun));
}
