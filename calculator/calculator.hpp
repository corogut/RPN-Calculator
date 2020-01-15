#pragma once
#include <map>
#include "tree.hpp"
class Calculator {
  double calc_result(Tree *t); // Computes the expression from tree rpn
public:
  double error_checker =1; // used to check if variables are defined in expression
  std::map<std::string_view, double>var_map; // Stores the variables and their value
  bool add_line(std::string_view var,std::string_view line); // Solves line expression and stores in
                                                             // var in expression is solvable
  double get_variable(std::string_view var); // Returns value of variable var
  bool is_defined(std::string_view var);     // check if var exist in calculator
  void clear(); // Clears the calculator of values and variables
  void operator=(const Calculator &rhs);
};
