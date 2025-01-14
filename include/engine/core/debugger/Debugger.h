/******************************************************************************
 * @file   Debugger.h
 * @author Valentin Dumitru
 * @date   2023-09-26
 * @brief A set of macros and functions for debugging and logging.
 * @details This file contains a set of macros and functions that can be used for debugging and logging purposes.
 * It includes debugging for SDL, LOG, and DEBUG_CODE macros.
 *
 * Copyright (c) 2023 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 ******************************************************************************/
#pragma once

#include <string>
#include "engine/core/logger/Logger.h"


#define SDLCall(call) call
#define postSDLCall


#ifndef NDEBUG_GAPI

#include "Stringer.h"
#include <string>
#include <iterator>
#include <sstream>
#include <type_traits>
#include <cstring>
#include <algorithm>

#ifndef NLOGGING
#define LOG(initialMessage, CODE)            \
    do{                                      \
    Logger::get().info(initialMessage);      \
        CODE                                 \
    } while(0)

#define LOG_BASIC(CODE)                      \
    do{                                      \
        CODE                                 \
    } while(0)

#endif


/**
 * @brief Macro to call an SDL function and check for errors.
 */
#define SDLCall(call)                                                 \
    do {                                                              \
        call;                                                         \
        const char* error = SDL_GetError();                           \
        D_ASSERT_TRUE(*error == '\0',                                 \
           "Error when calling SDL function: "                        \
           #call " " + std::string(error));                           \
    } while (0)

/**
 * @brief Macro to call after an SDL function to check for errors. It's used instead of SDLCall if the function
 * returns a value.
 */
#define postSDLCall                                                  \
    const char* error = SDL_GetError();                              \
    D_ASSERT_TRUE(*error == '\0',                                    \
         "Error post SDL function" + std::string(error));
/**
* @brief Converts the last argument of a variadic template into a string.
*
* This is the base case for the variadic template recursion. It processes the
* final argument, extracting its name and converting its value to a string.
*
* @tparam T The type of the argument.
* @param names A comma-separated list of argument names as a C-string.
* @param value The value of the argument to be converted to string.
* @return std::string A formatted string representing the argument's name and value.
*/
template <typename T>
std::string argsToString(const char* names, T value) {
    std::ostringstream arg;
    const char* nameEnd = std::strchr(names, ',');
    std::string name(names, nameEnd ? nameEnd - names : std::strlen(names));

    arg << name << ": " << GLESC::Stringer::toString(value);
    std::string result = arg.str();

    // Replace '\n' with a space (or any other character/string)
    std::replace(result.begin(), result.end(), '\n', ' ');

    return result;
}

/**
 * @brief Converts each argument of a variadic template into a string.
 *
 * This function processes each argument of a variadic template, converting
 * them into a string format. It is recursively called for each argument
 * until only one is left, at which point the base case function is invoked.
 *
 * @tparam T The type of the first argument.
 * @tparam Args The types of the remaining arguments.
 * @param names A comma-separated list of argument names as a C-string.
 * @param value The value of the first argument.
 * @param args The remaining arguments.
 * @return std::string A formatted string representing all arguments' names and values.
 */
template <typename T, typename... Args>
std::string argsToString(const char* names, T value, Args... args) {
    std::ostringstream arg;
    const char* nameEnd = std::strchr(names, ',');
    std::string name(names, nameEnd ? nameEnd - names : std::strlen(names));

    arg << name << ": " << GLESC::Stringer::toString(value);

    // Proceed to the next argument, if any
    if (nameEnd && *nameEnd == ',') {
        nameEnd++; // Skip the comma
        while (*nameEnd == ' ')
            nameEnd++; // Skip any whitespace after the comma
        // Recursively process the remaining arguments
        return arg.str() + ", " + argsToString(nameEnd, args...);
    }
    return arg.str();
}
/**
 * @brief Converts the function name and its arguments into a string.
 *
 * This function is used to convert the function name and its arguments into a
 * string format. It is used to log the function name and its arguments when
 * entering a function.
 *
 * @param FUNCTION_NAME The name of the function.
 * @param ... The arguments of the function.
 * @return std::string A formatted string representing the function name and its arguments.
 */
#define FUNCTION_CALL_STR(FUNCTION_NAME, ...) \
        std::string(FUNCTION_NAME) + \
        std::string("(")+                     \
            argsToString(#__VA_ARGS__, __VA_ARGS__)                     \
        +std::string(")")
#endif
