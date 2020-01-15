#include "calculator.hpp"
#include "tree.hpp"
#include "scanner.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <stack>
#include <string_view>
#include <vector>

// Evaluate the expression in line and assigns the double result to the variable
// in var. Returns false if incorrect expression entered and tree was not created
// correctly. 
//Input: Insert the variable needed to be assigned result from computer expression of line 
//Output:Returns false if there are any errors encountered when calculating string else true
bool Calculator::add_line(std::string_view var, std::string_view line) {
  Tree *t = calculator(line); // Build tree from line expression
  if (t == 0) { // Check if there were issues with building the tree
    //std::cout << "ERROR: Tree not built correctly\n";
    //delete t;
    return false;
  }
  double result = calc_result(t); // get the result from evaluating tree rpn
  if (error_checker == 0) { // check if issues occured when solving expression
    error_checker = 1;      // Reset checker
    t->destroy_node(t);
    return false;
  }
  if (is_defined(var)) { // Check if var value has to be overwritten since it
                         // exist in map
    var_map[var] = result;
    t->destroy_node(t);
    return true;
  }
  var_map.insert(std::pair<std::string_view, double>(var, result));  // var did not exist already so add new to map
  t->destroy_node(t); // free memory from tree
  return true;
}

// Return the result of the varible entered
// Input:Enter var that you want to find value of
// Output:Return double result of var if it exist, else 0 is returned
double Calculator::get_variable(std::string_view var) {
  double value = 0;
  if (is_defined(var)) {     // checks if thee variable is defined in the map
    value = var_map.at(var); // Value is defined
    //std::cout << "The value at " << var << " is " << value << "\n";
  }
  return value; // Value is not defined
}

// Checks if variable has been assigned before
// Input: Insert variable that needs to be check if it exist
// Output:Return true if var exist else false
bool Calculator::is_defined(std::string_view var) {
  std::map<std::string_view, double>::iterator it; // Used to iterate through
                                                   // map
  it = var_map.find(var);    // Try to find variable in map
  if (it != var_map.end()) { // Check if the iterator was able to find var
    // std::cout<<var<<" exist in calculator\n";
    return true; // Was able to find var
  }
  // std::cout<<var<<" is not defined in calculator\n";
  return false; // was not able to find var
}

// Clear the contents of all variables and are now undefined
void Calculator::clear() {
  var_map.clear();             // Map is cleared
  assert(var_map.size() == 0); // Size should be zero
}

// Create the = operator to make two calculators equal
void Calculator::operator=(const Calculator &rhs) {
  var_map = rhs.var_map; // Set the contents of the map equal to each other
}

// Calculate the result of the expression using the rpn of the tree built off
// that expression 
//Input:Enter tree created from line expression 
//Output:Return the result of the rpn string or 0 if error occured
double Calculator::calc_result(Tree *t) {
  t->get_rpn();
  std::string rpn = t->rpn;             // Get the rpn string for the tree
  std::stack<std::string> string_stack; // Used as a stack
  double results = 0;
  // Go through string to push operands and pop when operators appear
  for (unsigned int i = 0; i < rpn.length(); i++) {
    // std::cout<<results<<"\n";
    if ((rpn[i] >= 'A' && rpn[i] <= 'Z') ||(rpn[i] >= 'a' && rpn[i] <= 'z')) { // Checks if alphabetical string
      std::string var = "";  // used to store string
      while (rpn[i] != ' ' ||i >= rpn.length()) { // Since rpn is seprated by space, go though
                                                  // string until space is reached or end of string
        var = var + rpn[i]; // append to string
        i++;                // used to iterate through characters in string
      }
      // std::string_view var_view=var;
      if (is_defined(var)) { // Check if var already exist and gets its value
        double value = get_variable(var);
        string_stack.push(std::to_string(value));
        continue;
      } 
      else { // Variable is undefined so return 0 as an error
        //std::cout << "ERROR:variable " << var << " does not exist\n";
        error_checker = 0;
        return error_checker;
      }
      // std::cout<<var<<"\n";
    } 
    else if (rpn[i] >= '0' && rpn[i] <= '9') { // Check if string is number
      std::string num = "";
      while (rpn[i] != ' ' || i >= rpn.length()) { // Get entire number
        num = num + rpn[i];                        // append to temp string
        i++;                                       // iterate through string
      }
      // std::cout<<num<<"\n";
      string_stack.push(num); // Add number to stack
    }
    // Check for operators
    else if (rpn[i] == '*') {
      // Compute the multiplication
      std::string r = string_stack.top();
      double rhs = std::stod(r);
      string_stack.pop();
      std::string l = string_stack.top();
      double lhs = std::stod(l);
      string_stack.pop();
      double temp = lhs * rhs;
      string_stack.push(std::to_string(temp));
      i++; // skip the space
    } 
    else if (rpn[i] == '+') {
      // compute addition
      std::string r = string_stack.top();
      double rhs = std::stod(r);
      string_stack.pop();
      std::string l = string_stack.top();
      double lhs = std::stod(l);
      string_stack.pop();
      double temp = lhs + rhs;
      string_stack.push(std::to_string(temp));
      i++; // skip the space
    } 
    else if (rpn[i] == '-') {
      // compute subtraction
      std::string r = string_stack.top();
      double rhs = std::stod(r);
      string_stack.pop();
      std::string l = string_stack.top();
      double lhs = std::stod(l);
      string_stack.pop();
      double temp = lhs - rhs;
      string_stack.push(std::to_string(temp));
      i++; // skip the space
    } 
    else if (rpn[i] == '/') {
      // compute division
      std::string r = string_stack.top();
      double rhs = std::stod(r);
      string_stack.pop();
      std::string l = string_stack.top();
      double lhs = std::stod(l);
      string_stack.pop();
      double temp = lhs / rhs;
      string_stack.push(std::to_string(temp));
      i++; // skip the space
    } 
    else if (rpn[i] == '%') {
      // get remainder
      double rhs = std::stod(string_stack.top());
      string_stack.pop();
      double lhs = std::stod(string_stack.top());
      string_stack.pop();
      double temp = (int)lhs % (int)rhs;
      string_stack.push(std::to_string(temp));
      i++; // skip the space
    }
    std::string top = string_stack.top();
    results = std::stod(top);
    // std::cout<<"Current result: "<<results<<"\n";
  }
  // std::cout<<"End result is "<<results<<"\n";
  return results;
}
