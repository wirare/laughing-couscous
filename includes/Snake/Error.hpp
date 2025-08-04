#pragma once

#include <stdexcept>
#include <execinfo.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cxxabi.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define LOCATION(file, func, line) \
    std::string("In file: ") + file + \
    "\nIn function: '" + func + "'" + \
    "\nAt line: " + STR(line)

#define EXCEPTION(except, msg) \
	except(std::string(msg) + "\n" + getCaller() + "\n" + LOCATION(__FILE__, __func__, __LINE__))

#define OUT_OF_BOUNDS EXCEPTION(std::invalid_argument, "Out of bounds arguments")
#define SQUARE_NOT_EMPTY EXCEPTION(std::invalid_argument, "Square is not empty")
#define SNAKE_ON_SQUARE EXCEPTION(std::invalid_argument, "Snake already on square")
#define LAYER_TYPE_ERROR EXCEPTION(std::runtime_error, "Mismatch between layers type")
#define FAIL_O_S_MODEL EXCEPTION(std::runtime_error, "Failed to open file for saving model")
#define FAIL_O_L_MODEL EXCEPTION(std::runtime_error, "Failed to open file for loading model")
#define DIM_ERR_DENSE EXCEPTION(std::runtime_error, "Dimension error in Dense layer")
#define UNKNOWN_LAYER EXCEPTION(std::runtime_error, "Unknown LayerType in model file")
#define READING_ERR EXCEPTION(std::runtime_error, "Error while reading model file")

inline std::string getCaller() {
    void* callstack[10];
    int frames = backtrace(callstack, 10);
    char** symbols = backtrace_symbols(callstack, frames);

    std::ostringstream oss;
    if (frames >= 3) {
        std::string symbol(symbols[2]); // Frame 2 is usually the caller

        size_t begin = symbol.find('(');
        size_t end = symbol.find('+', begin);
        if (begin != std::string::npos && end != std::string::npos) {
            std::string mangled = symbol.substr(begin + 1, end - begin - 1);

            int status;
            char* demangled = abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &status);
            if (status == 0 && demangled != nullptr) {
                oss << "Caller: " << demangled;
                std::free(demangled);
            } else {
                oss << "Caller (mangled): " << mangled;
            }
        } else {
            // fallback: show the full symbol if parsing fails
            oss << "Caller (symbol): " << symbol;
        }
    } else {
        oss << "Caller: Unknown";
    }

    std::free(symbols);
    return oss.str();
}
