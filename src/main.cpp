#include "core/number.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <stdexcept>

enum class Format { Integer, Binary };

// UI Helpers
void print_separator(char c = '=') {
  std::cout << std::string(60, c) << "\n";
}

void print_help() {
  std::cout << "Usage: collatz [OPTIONS] <NUMBER>\n\n"
            << "Arguments:\n"
            << "  <NUMBER>      Starting integer (e.g., 27) or binary (e.g., 0b11011)\n\n"
            << "Options:\n"
            << "  -d, --decimal Print steps as integers (Base 10) [Default]\n"
            << "  -b, --binary  Print steps as binary (Base 2)\n"
            << "  -q, --quiet   Suppress steps, print only execution stats\n"
            << "  -h, --help    Show this help message.\n";
}

int main(int argc, char* argv[]) {
  // Optimize standard I/O operations
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  std::string input_str = "";
  Format output_format  = Format::Integer;
  bool quiet            = false;

  // Argument Parsing
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "--help") {
      print_help();
      return 0;
    }
    else if (arg == "-b" || arg == "--binary") {
      output_format = Format::Binary;
    }
    else if (arg == "-d" || arg == "--decimal") {
      output_format = Format::Integer;
    }
    else if (arg == "-q" || arg == "--quiet") {
      quiet = true;
    }
    else if (input_str.empty()) {
      input_str = arg;
    }
    else {
      std::cerr << "Error: Unknown argument provided: " << arg << "\n";
      return 1;
    }
  }

  // Validation
  if (input_str.empty()) {
    std::cerr << "Error: No input provided. Run with --help for usage.\n";
    return 1;
  }

  Number current;
  try {
    current = Number(input_str);
  } catch (const std::exception& e) {
    std::cerr << "Error parsing input: " << e.what() << "\n";
    return 1;
  }

  if (current.is_zero()) {
    std::cerr << "Error: Collatz conjecture applies to positive integers.\n";
    return 1;
  }

  // Execution Setup
  Number peak          = current;
  uint64_t total_steps = 0;
  uint64_t even_steps  = 0;
  uint64_t odd_steps   = 0;

  auto format_number = [&](const Number& n) {
    return (output_format == Format::Binary) ? n.to_string_bin() : n.to_string_dec();
  };

  print_separator('=');
  std::cout << " COLLATZ CONJECTURE SOLVER \n";
  print_separator('=');
  std::cout << "Input  : " << format_number(current) << "\n";
  std::cout << "Format : " << (output_format == Format::Binary ? "Binary" : "Integer") << "\n";
  print_separator('-');

  auto start_time = std::chrono::high_resolution_clock::now();

  // Core Logic Loop
  while (!current.is_one()) {
    if (!quiet) {
      std::cout << "Step " << std::left << std::setw(6) << total_steps << ": "
                << format_number(current) << "\n";
    }

    if (current.is_even()) {
      current.step_even();
      even_steps++;
    } else {
      current.step_odd();
      odd_steps++;
    }

    if (peak < current) {
      peak = current;
    }

    total_steps++;
  }

  // Final Output
  if (!quiet) {
    std::cout << "Step " << std::left << std::setw(6) << total_steps << ": "
              << format_number(current) << "\n";
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = end_time - start_time;

  print_separator('-');
  std::cout << "Execution Stats:\n";
  std::cout << " - Total Steps : " << total_steps << "\n";
  std::cout << " - Even Steps  : " << even_steps << "\n";
  std::cout << " - Odd Steps   : " << odd_steps << "\n";
  std::cout << " - Peak Value  : " << format_number(peak) << "\n";
  std::cout << " - Time Taken  : " << std::fixed << std::setprecision(4) << duration.count() << " ms\n";
  print_separator('=');

  return 0;
}