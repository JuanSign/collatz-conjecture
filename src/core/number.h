#ifndef NUMBER_H
#define NUMBER_H

#include <cstdint>
#include <string>
#include <vector>

class Number {
private:
  // Limbs stored in little-endian format (limbs[0] is least significant)
  std::vector<uint32_t> limbs;

  // Internal Helpers
  void     mul10_add(uint32_t digit);
  uint32_t div10();

public:
  // Constructors
  Number();
  explicit Number(const std::string& str);

  // State Checks
  bool is_one()  const;
  bool is_zero() const;
  bool is_even() const;

  // Collatz Operations
  void step_even();
  void step_odd();

  // Operator Overloads
  bool operator<(const Number& other) const;

  // Output Generators
  std::string to_string_dec() const;
  std::string to_string_bin() const;
};

#endif