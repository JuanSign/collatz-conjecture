#include "number.h"

#include <algorithm>
#include <stdexcept>

// Constructors
Number::Number() : limbs(1, 0) {}

Number::Number(const std::string& str) {
  if (str.empty()) {
    limbs.push_back(0);
    return;
  }

  bool is_binary_string = (str.size() >= 2 && str[0] == '0' && (str[1] == 'b' || str[1] == 'B'));

  if (is_binary_string) {
    // Parse Binary String
    int bit_count = 0;
    uint32_t current_limb = 0;

    for (auto it = str.rbegin(); it != str.rend() - 2; ++it) {
      if (*it == '1') {
        current_limb |= (1U << bit_count);
      } else if (*it != '0') {
        throw std::invalid_argument("Invalid character in binary string.");
      }

      bit_count++;

      if (bit_count == 32) {
        limbs.push_back(current_limb);
        current_limb = 0;
        bit_count = 0;
      }
    }

    if (bit_count > 0 || limbs.empty()) {
      limbs.push_back(current_limb);
    }

  } else {
    // Parse Decimal String
    for (char c : str) {
      if (c < '0' || c > '9') {
        throw std::invalid_argument("Invalid character in decimal string.");
      }
      mul10_add(c - '0');
    }

    if (limbs.empty()) {
      limbs.push_back(0);
    }
  }

  // Normalize by stripping leading zeros
  while (limbs.size() > 1 && limbs.back() == 0) {
    limbs.pop_back();
  }
}

// Internal Math Helpers
void Number::mul10_add(uint32_t digit) {
  uint64_t carry = digit;

  for (size_t i = 0; i < limbs.size(); ++i) {
    uint64_t prod = (static_cast<uint64_t>(limbs[i]) * 10ULL) + carry;
    limbs[i]      = static_cast<uint32_t>(prod);
    carry         = prod >> 32;
  }

  if (carry > 0) {
    limbs.push_back(static_cast<uint32_t>(carry));
  }
}

uint32_t Number::div10() {
  uint64_t rem = 0;

  for (auto it = limbs.rbegin(); it != limbs.rend(); ++it) {
    uint64_t val = (rem << 32) | *it;
    *it          = static_cast<uint32_t>(val / 10ULL);
    rem          = val % 10ULL;
  }

  while (limbs.size() > 1 && limbs.back() == 0) {
    limbs.pop_back();
  }

  return static_cast<uint32_t>(rem);
}

// State Checks
bool Number::is_one()  const { return limbs.size() == 1 && limbs[0] == 1; }
bool Number::is_zero() const { return limbs.size() == 1 && limbs[0] == 0; }
bool Number::is_even() const { return (limbs[0] & 1) == 0; }

// Collatz Operations
void Number::step_even() {
  uint32_t carry = 0;

  for (auto it = limbs.rbegin(); it != limbs.rend(); ++it) {
    uint32_t next_carry = (*it & 1) ? 0x80000000 : 0;
    *it   = (*it >> 1) | carry;
    carry = next_carry;
  }

  if (limbs.size() > 1 && limbs.back() == 0) {
    limbs.pop_back();
  }
}

void Number::step_odd() {
  uint64_t carry = 1; // Pre-load the "+ 1"

  for (size_t i = 0; i < limbs.size(); ++i) {
    uint64_t prod = (static_cast<uint64_t>(limbs[i]) * 3ULL) + carry;
    limbs[i]      = static_cast<uint32_t>(prod);
    carry         = prod >> 32;
  }

  if (carry > 0) {
    limbs.push_back(static_cast<uint32_t>(carry));
  }
}

// Operator Overloads
bool Number::operator<(const Number& other) const {
  if (limbs.size() != other.limbs.size()) {
    return limbs.size() < other.limbs.size();
  }

  for (auto it1 = limbs.rbegin(), it2 = other.limbs.rbegin();
       it1 != limbs.rend(); ++it1, ++it2) {
    if (*it1 != *it2) {
      return *it1 < *it2;
    }
  }

  return false;
}

// Output Generators
std::string Number::to_string_dec() const {
  if (is_zero()) return "0";

  Number temp = *this;
  std::string res;

  while (!temp.is_zero()) {
    res.push_back('0' + temp.div10());
  }

  std::reverse(res.begin(), res.end());
  return res;
}

std::string Number::to_string_bin() const {
  if (is_zero()) return "0b0";

  std::string res = "";

  for (auto it = limbs.rbegin(); it != limbs.rend(); ++it) {
    std::string bits = "";
    uint32_t val = *it;

    for (int i = 0; i < 32; ++i) {
      bits.push_back((val & 1) ? '1' : '0');
      val >>= 1;
    }

    std::reverse(bits.begin(), bits.end());

    if (it == limbs.rbegin()) {
      size_t first_one = bits.find('1');
      if (first_one != std::string::npos) {
        res += bits.substr(first_one);
      }
    } else {
      res += bits;
    }
  }

  return "0b" + res;
}