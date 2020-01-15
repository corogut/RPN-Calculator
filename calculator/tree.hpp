#pragma once
#include <string>
#include <string_view>
class Tree {
public:
  std::string txt;     // Stores the text of node. Kept as string so it can be appended
  int tok;     // The token type of node
  int len;     // Length of txt
  Tree *left;  // Left child of current node
  Tree *right; // Right child of current node
  std::string rpn; // Keeps current position of rpn string.Used to be able to
                   // append other strings to it
  int parent;      // Used for check if there are parents to this node

  Tree(int l, int t);
  void destroy_node(Tree *t); // Used to destroy the entire tree
  void traverse_depth_first(void (*fun)(const Tree *));   // Traverse tree in depth first preorder
  std::string_view get_txt() const; // Return txt of node
  std::string_view get_tok() const; // Return token type of node
  void get_rpn();                   // Fills in the rpn of tree
  void traverse_post_order(const Tree *t); // Traverse the tree using preorder.
                                           // Used to help for getting rpn
};
