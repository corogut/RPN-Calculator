#include "scanner.hpp"
#include <iostream>
#include <string_view>
#include <vector>

Tree *parse_operand(std::vector<Tree *> token_list, int num_of_tokens);
Tree *parse_mult(std::vector<Tree *> token_list, int num_of_tokens);
Tree *parse_add(std::vector<Tree *> token_list, int num_of_tokens);

int position = 0; // Keeps track of position throughout entire program
// Scanner that breaks up entered string into tokens and creates a tree based on
// these tokens. Tree has operator as nodes and operands as leafs
// Input:Takes in string expression. Currently only handles +,*,(,),variables,
// and numbers
// Output:Returns built tree
Tree *calculator(std::string_view text) {
  std::string_view temp = text;
  position = 0;                   // reset variable
  std::vector<Tree *> token_list; // Used to store tree nodes in a vector
  // const char *temp = text.c_str(); // Convert C++ string into C string to
  // iterate through and add null ending
  int num_of_tokens = 0; // Keeps track of number of tokens in expression
  int par_check = 0;     // Checks if parenthese all have a pair
  for (unsigned int i = 0; i < temp.size();i++) { // Goes through string text and checks if reached end of line
    // Check for comments
    if (temp[i] == '/' && temp[i + 1] == '*') { // Used to check for block comments
      i = i + 2;              // skip over the comment characters
      int comment = 1;
      while (comment == 1) { // iterate through line until end of comment or line is reached
        if (temp[i] == '*' && temp[i + 1] == '/') {
          i++;
          break;
        }
        i++;                    // Skip contents inside comments
        if (i >= temp.size()) { // end of line reached
          break;
        }
      }
    } 
    else if (temp[i] == '/' && temp[i + 1] == '/') { // checks for line comments
      i = i + 2;
      while (temp[i] != '\n') { // Skip expression until new line or end of line
        i++;
        if (i >= temp.size()) { // end of line reached
          break;
        }
      }
    }
    // Checks if starting letter is alphabetical
    else if ((temp[i] >= 'a' && temp[i] <= 'z') ||(temp[i] >= 'A' && temp[i] <= 'Z')) {
      std::string var = ""; // Used as storage for the variable
      int len = 0; // Counter to go through found variable and find length of
                   // substring
      // Goes through variable looking for either letter or number.i+len used to
      // jump over checked words
      while ((temp[i + len] >= 'a' && temp[i + len] <= 'z') ||(temp[i + len] >= 'A' && temp[i + len] <= 'Z') ||(temp[i + len] >= '0' && temp[i + len] <= '9')) {
        var = var + temp[i + len]; // Append chars to this variable
        len++;                     // Size of substring increases
      }
      num_of_tokens++;            // Increase number of found tokens
      Tree *t = new Tree(len, 1); // create a new tree node
      t->txt = var;
      token_list.push_back(t); //  add new tree node to vector
      i = len + i -1; // Change i counter and subtract 1 to make up for i++ in for loop
      // std::cout<<var<<"\n";
    }

    // Checks if word is a number
    else if (temp[i] >= '0' && temp[i] <= '9') {
      int len = 0; // Counter to go through found number and find length
      std::string num = ""; // Used for number substring
      // Goes through digit and check for either letter or number
      while ((temp[i + len] >= 'a' && temp[i + len] <= 'z') ||(temp[i + len] >= 'A' && temp[i + len] <= 'Z') ||(temp[i + len] >= '0' && temp[i + len] <= '9')) {
        num = num + temp[i + len]; // Append char digits to num
        len++;
      }
      num_of_tokens++;            // Increase num of tokens found
      Tree *t = new Tree(len, 2); // Create new tree node
      t->txt = num;
      token_list.push_back(t); // Add new tree node to back of vector
      i = len + i - 1;         // Skips word created
      // std::cout<<num<<"\n";
    }

    // Checks for whitespaces and skips them
    else if ((int)temp[i] == 32 || ((int)temp[i] >= 9 && (int)temp[i] <= 13)) {
      continue; // Continue with the for loop
    }

    // Check for single operators
    else if (temp[i] == '(' || temp[i] == ')' || temp[i] == '+' || temp[i] == '*' || temp[i] == '/' || temp[i] == '-' || temp[i] == '%') {
      if (temp[i] == '(') { // Checks if parenthese
        par_check++;        // Increment counter. Should be zero when all tokens
                            // created
      }
      if (temp[i] == ')') {
        par_check--; // Reduce counter, pair found
      }
      std::string op = "";
      op = op + temp[i]; // Append to this string
      num_of_tokens++;
      Tree *t = new Tree(1, 3); // Add operator to token array
      t->txt = op;
      token_list.push_back(t);
      // std::cout<<op<<"\n";
    }

    // Invalid token or token that is not used in this lab
    else {
      // std::cout<<"Not valid"<<"\n";
      for (int i = 0; i < num_of_tokens;i++) { // delete currently built token array
        delete token_list.at(i);
      }
      return 0;
    }
  }
  if (num_of_tokens == 0 ||par_check !=0) { // Empty string to function or there are extra parenthese
    if (par_check != 0) {
      for (int i = 0; i < num_of_tokens; i++) {
        delete token_list.at(i);
      }
    }
    return 0;
  }
  position = 0; // Make sure position index is reset
  Tree *results =parse_add(token_list, num_of_tokens); // Creates the tree from the tokens
                                                       // and returns root of tree
  if (results == 0) {                                  // tree could not be built
    for(unsigned int i=0;i<token_list.size();i++){
      delete token_list.at(i);
    }
    return 0;
  }
  results->parent = 0;          // Set root parent to 0. Helpful for traversing depth first
  return results; // Returns the root of the tree
}

// Used to check for numbers, variables, and (,) in the token array
// Input:Takes in the array of tree node objects created from text.Also has
// number of tokens in array
// Output: returns tree pointer with appropriate children
Tree *parse_operands(std::vector<Tree *> token_list, int num_of_tokens) {
  if(position>=num_of_tokens){//end of line reached
    return 0;
  }
  Tree *op = token_list.at(position); // Used to check current token
  Tree *expr; // Used to store expressions when () are found
  if (op->tok == 1 || op->tok == 2) { // Check if number or variable
    // std::cout<<"Parsed operand: "<<op->get_txt()<<"at position:
    // "<<position<<"\n";
    position++;
    return op;
  } 
  else if (op->txt == "(") { // Check if this statement is in parenthese
        position++; // Go to next token and ignore ( in the tree
    expr = parse_add(token_list,num_of_tokens); // Start creating the expression inside parenthese
    if (token_list.at(position)->txt ==")") { // Makes sure there is an end bracket
      delete op; 
      delete token_list.at(position);
      position++;  // increase position in array
      return expr; // Return subexpression
    }
  }
  //std::cout<< "ERROR:Missing Bracket or incorrect expression\n";
  return 0;
}

// Used to check for multiplation symbols in token node array
// Input:Takes in token list and the size of the array
Tree *parse_mult(std::vector<Tree *> token_list, int num_of_tokens) {
  if(position>=num_of_tokens){//no operand on rhs
    return 0;
  }
  Tree *lhs =parse_operands(token_list, num_of_tokens); // Check and get operand
  if (lhs == 0) {
    return 0; // error when parsing expression
  }
  if (position >= num_of_tokens) { // Check if the end of array has been reached
    return lhs;
  }
  Tree *t = token_list.at(position); // peek into next tree node
  if(t->tok==1||t->tok==2){//Back to back numbers or variables with no operator
    return 0;
  }
  while (t->txt == "*" || t->txt == "/" ||t->txt == "%") { // Check if operator is multiplication
    position++;           // Consume position
    Tree *rhs = parse_operands(token_list, num_of_tokens); // Get the operand for right hand side
    if (rhs == 0) {
      return 0; // error when parsing the tree. Incorrect expression
    }
    t->left = lhs;  // Set the left ptr for t
    t->right = rhs; // Set right ptr for t
    lhs = t;                         // Set the left hand ptr to equal t
    if (position >= num_of_tokens) { // end of array reached
      // std::cout<<"MAXED\n";
      break;
    }
    t = token_list.at(position); // peak into next element in array
  }
  return lhs; // Return this curerent expression
}

// Used to check for addition symbol in expression and pays attenetion to
// operator precedence
// Input:Takes in token array of tree node and size of the array
Tree *parse_add(std::vector<Tree *> token_list, int num_of_tokens) {
  if(position>=num_of_tokens){//operator has no rhs to evaluate
    return 0;
  }
  Tree *lhs = parse_mult(token_list, num_of_tokens); // Check and get operand
  if (lhs == 0) { // Could not parse tree expression
    return 0;
  }
  if (position >= num_of_tokens) { // Check if end of array has been reached
    return lhs;
  }
  Tree *t = token_list.at(position);       // peek into next tree node
  if(t->tok==1||t->tok==2){//Back to back numbers or variables
    return 0;
  }
  while (t->txt == "+" || t->txt == "-") { // Check if t is the addition symbol
    position++;                            // Consume position
    Tree *rhs = parse_mult(token_list, num_of_tokens); // Set right hand side to multiplication
                                                       // expression due to operator precedence
    if (rhs == 0) {
      return 0;
    }
    t->left = lhs; // Set the children of the t node
    t->right = rhs;
    // std::cout<<"Top: "<<t->get_txt()<<"\n";
    // std::cout<<"Left: "<<lhs->get_txt()<<"\n";
    // std::cout<<"Right: "<<rhs->get_txt()<<"\n";
    lhs = t;
    if (position >= num_of_tokens) { // Check if reached end of array
      // std::cout<<"MAXED\n";
      break;
    }
    t = token_list.at(position); // peak into next
  }
  // std::cout<<lhs->get_txt()<<"\n";
  return lhs; // Return this expression that was created
}
// Used for debugging
void print_node(const Tree *n) { 
  std::cout << n->get_txt() << "\n"; 
}
