/**************************************************************************************************
 * @file   Stringer.h
 * @author Valentin Dumitru
 * @date   2023-11-22
 * @brief  Add description of this file if needed @TODO 
 *
 * Copyright (c) 2023 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/
#pragma once

#include <string>
#include <sstream>
#include <limits>
#include <type_traits>
#include <iomanip>
#include <cstdint>
#include <utility>

namespace GLESC {
    template <typename T, typename = void>
    struct isIterable : std::false_type {};

    template <typename T>
    struct isIterable<T, std::void_t<decltype(std::declval<T>().begin()),
                                     decltype(std::declval<T>().end())>> : std::true_type {};

    template <typename T>
    constexpr bool isIterable_v = isIterable<T>::value;

    // Helper to deduce the return type of toString if exists
    template <typename T>
    auto checkToString(int) -> decltype(std::declval<T>().toString(), std::true_type{});

    // Fallback if toString does not exist
    template <typename T>
    std::false_type checkToString(...);

    // Trait to check if toString exists
    template <typename T>
    struct hasToString : decltype(checkToString<T>(0)) {};

    template <typename T>
    constexpr bool hasToString_v = hasToString<T>::value;

    class Stringer {
        template <typename Type>
        static std::string nonIterableToString(const Type& value) {
            std::ostringstream oss;

            if constexpr (std::is_pointer_v<Type>) {
                if constexpr (std::is_void_v<std::remove_const_t<std::remove_pointer_t<Type>>>) {
                    // Format void* pointers as hexadecimal memory addresses
                    oss << "void*: 0x" << std::hex << std::setw(sizeof(void*) * 2) << std::setfill('0')
                        << reinterpret_cast<uintptr_t>(value);
                }
                else {
                    return nonIterableToString(*value);
                }
            }
            else if constexpr (std::is_array_v<Type>) {
                oss << "[";
                for (size_t i = 0; i < std::extent_v<Type>; ++i) {
                    if (i != 0) {
                        oss << ", ";
                    }
                    oss << nonIterableToString(value[i]);
                }
                oss << "]";
            }
            else if constexpr (std::is_enum_v<Type>) {
                // Handle enums by casting to their underlying type
                oss << static_cast<std::underlying_type_t<Type>>(value);
            }
            else if constexpr (std::is_convertible_v<Type, std::string_view>) {
                oss << value;
            }
            else if constexpr (std::is_null_pointer_v<Type>) {
                oss << "nullptr";
            }
            else if constexpr (std::is_floating_point_v<Type>) {
                // Handle floating points with more precision
                oss << std::fixed << std::setprecision(std::numeric_limits<Type>::max_digits10)
                    << value;
            }
            else if constexpr (std::is_arithmetic_v<Type>) {
                // Directly use std::to_string for arithmetic types
                return std::to_string(value);
            }
            else if constexpr (hasToString_v<Type>) {
                // Handle types with a toString() method
                oss << value.toString();
            }
            else {
                // TODO: Handle other types
            }

            return oss.str();
        }

        template <typename Iterable>
        static std::string iterableToString(const Iterable& iterable) {
            std::ostringstream oss;
            oss << "{";
            for (auto it = iterable.begin(); it != iterable.end(); ++it) {
                if (it != iterable.begin()) {
                    oss << ", ";
                }
                oss << toString(*it);
                if(isIterable_v<decltype(*it)>){
                    oss << "\n";
                }


            }
            oss << "}";
            return oss.str();
        }

    public:
        template <typename Type>
        static std::string toString(const Type& value) {
            if constexpr (isIterable_v<Type> && !std::is_same_v<Type, std::string>) {
                return iterableToString(value);
            }
            else {
                return nonIterableToString(value);
            }
        }
    }; // class Stringer
} // namespace GLESC