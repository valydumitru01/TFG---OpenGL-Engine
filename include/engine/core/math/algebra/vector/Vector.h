/******************************************************************************
 * @file   Vector.h
 * @author Valentin Dumitru
 * @date   2023-09-26
 * @brief @todo Add description of this file if needed
 *
 * Copyright (c) 2023 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 ******************************************************************************/

#pragma once

#include <type_traits>
#include <cmath>
#include <string>
#include "engine/core/math/Math.h"
#include "engine/core/math/asserts/VectorAsserts.h"
#include "engine/core/math/algebra/vector/VectorAlgorithms.h"
#include "engine/core/math/algebra/vector/VectorMixedAlgorithms.h"


namespace GLESC::Math {
#pragma pack(push, 1)  // Push the current alignment to the stack and set new alignment to 1 byte
    template <typename Type, size_t N>
    [[nodiscard]] class Vector {
    public:
        using ValueType = Type;
        // =========================================================================================
        // ====================================== Constructors =====================================
        // =========================================================================================

        /**
         * @brief Default constructor
         * @details Initializes the vector with zeroes
         */
        constexpr Vector() noexcept {
            VectorAlgorithms::vectorZero(data);
        }

        /**
         * @brief Array constructor
         * @details Initializes the vector with the values from the array
         * @param list The array of values to initialize the vector with
         */
        constexpr Vector(const Type (&list)[N]) noexcept {
            VectorAlgorithms::vectorInitRawArray(data, list);
        }

        /**
         * @brief Variadic constructor
         * @details Initializes the vector with the values from the variadic arguments.
         * This means that this constructor can be called with any number of arguments.
         * This constructor is only enabled if the number of arguments is equal to the
         * dimension of the vector.
         * @tparam Args
         * @param args
         */
        template <typename... Args, typename = std::enable_if_t<(sizeof...(Args) == N)>>
        constexpr Vector(Args&&... args) noexcept : data{args...} {
        }


        /**
         * @brief Fill constructor
         * @details Initializes all the values of the vector with the given value
         * @param values
         */
        constexpr Vector(Type values) noexcept {
            VectorAlgorithms::vectorFill(data, values);
        }

        /**
         * @brief Move constructor for VectorData
         * @see VectorData
         * @param values The VectorData to move from
         */
        constexpr Vector(VectorData<Type, N>&& values) noexcept {
            VectorAlgorithms::vectorMove(data, values);
        }

        /**
         * @brief Copy constructor for VectorData
         * @see VectorData
         * @param values The VectorData to copy from
         */
        constexpr Vector(const VectorData<Type, N>& values) noexcept {
            VectorAlgorithms::vectorCopy(data, values);
        }


        /**
         * @brief Copy constructor
         * @details Copies the data from the other vector to this one
         * @param other The vector to copy from
         */
        template <size_t N2>
        constexpr Vector(const Vector<Type, N2>& other) noexcept {
            VectorAlgorithms::vectorFill(data, Type{0});
            VectorAlgorithms::vectorCopy(data, other.data);
        }

        /**
         * @brief Move constructor, for vectors of different sizes
         * @details Moves the data from the other vector to this one
         * @param other The vector to move from
         */
        template <size_t N2>
        constexpr Vector(const Vector<Type, N2>&& other) noexcept {
            VectorAlgorithms::vectorFill(data, Type{0});
            VectorAlgorithms::vectorMove(data, other.data);
        }

        /**
         * @brief Copy constructor
         * @details Copies the data from the other vector to this one
         * @param other The vector to copy from
         */
        constexpr Vector(const Vector& other) noexcept {
            VectorAlgorithms::vectorCopy(data, other.data);
        }

        /**
         * @brief Move constructor
         * @details Moves the data from the other vector to this one
         * @param other The vector to move from
         */
        constexpr Vector(Vector&& other) noexcept {
            VectorAlgorithms::vectorMove(data, other.data);
        }


        // =========================================================================================
        // ======================================== Accessors ======================================
        // =========================================================================================

        // ================================== General Accessors ====================================
        /**
         * @brief Returns the data of the vector given an index
         * @param index The index of the data to return
         * @return The data of the vector at the given index
         */
        Type& operator[](size_t index) {
            return data[index];
        }

        /**
         * @brief Returns the data of the vector given an index, const version (read only)
         * @param index
         * @return
         */
        const Type& operator[](size_t index) const {
            return data[index];
        }

        /**
         * @brief Returns the data of the vector given an index
         * @details This is the same as operator[] but it's more explicit (it's const, read only)
         * @param index
         * @return
         */
        const Type& get(size_t index) const {
            return data[index];
        }

        /**
         * @brief Sets the data of the vector given an index
         * @details This is the same as operator[] but it's more explicit.
         * Because it's a normal setter.
         * @param index
         * @param value
         */
        void set(size_t index, Type value) {
            data[index] = value;
        }

        // ================================== Special Accessors ====================================

        /**
         * @brief Returns the x (reference) value of the vector.
         * @details This is the same as operator[] but only for the x (first) value.
         * The returned value is a reference so it can be modified.
         * @return The x (first) value of the vector
         */
        [[nodiscard]] Type& x() {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(1);
            return data[0];
        }

        /**
         * @brief Sets the x (first) value of the vector
         * @details This works as a setter but for the x (first) value of the vector.
         * @param value The value to set
         */
        void x(Type value) {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(1);
            data[0] = value;
        }

        /**
         * @brief Returns the x (first) value of the vector
         * @details This is a getter for the x (first) value of the vector.
         * @return The x (first) value of the vector
         */
        [[nodiscard]] const Type& getX() const {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(1);
            return data[0];
        }

        /**
         * @brief Sets the x (first) value of the vector
         * @details This works as a setter but for the x (first) value of the vector.
         * @param value The value to set
         */
        void setX(Type value) {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(1);
            data[0] = value;
        }

        /**
         * @brief Returns the y (second) value of the vector.
         * @details This is the same as operator[] but only for the y (second) value.
         * The returned value is a reference so it can be modified.
         * @return The y (second) value of the vector
         */
        [[nodiscard]] Type& y() {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(2);
            return data[1];
        }

        /**
         * @brief Sets the y (second) value of the vector.
         * @details This works as a setter but for the y (second) value of the vector.
         * @param value The value to set
         */
        void y(Type value) {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(2);
            data[1] = value;
        }

        /**
         * @brief Returns the y (second) value of the vector.
         * @details This is a getter for the y (second) value of the vector.
         * @return The y (second) value of the vector
         */
        [[nodiscard]] Type getY() const {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(2);
            return data[1];
        }

        /**
         * @brief Sets the y (second) value of the vector.
         * @details This works as a setter for the y (second) value of the vector.
         * @param value The value to set
         */
        void setY(Type value) {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(2);
            data[1] = value;
        }

        /**
         * @brief Returns the z (third) value of the vector.
         * @details This is the same as operator[] but only for the z (third) value.
         * The returned value is a reference so it can be modified.
         * @return The z (third) value of the vector
         */
        [[nodiscard]] Type& z() {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(3);
            return data[2];
        }

        /**
         * @brief Sets the z (third) value of the vector
         * @details This works as a setter but for the z (third) value of the vector.
         * @param value The value to set
         */
        void z(Type value) {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(3);
            data[2] = value;
        }

        /**
         * @brief Returns the z (third) value of the vector
         * @details This is a getter for the z (third) value of the vector.
         * @return The z (third) value of the vector
         */
        [[nodiscard]] Type getZ() const {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(3);
            return data[2];
        }

        /**
         * @brief Sets the z (third) value of the vector
         * @details This works as a setter for the z (third) value of the vector.
         * @param value The value to set
         */
        void setZ(Type value) {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(3);
            data[2] = value;
        }

        /**
         * @brief Returns the w (fourth) value of the vector.
         * @details This is the same as operator[] but only for the w (fourth) value.
         * The returned value is a reference so it can be modified.
         * @return The w (fourth) value of the vector
         */
        [[nodiscard]] Type& w() {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(4);
            return data[3];
        }

        /**
         * @brief Sets the w (fourth) value of the vector
         * @details This works as a setter but for the w (fourth) value of the vector.
         * @param value The value to set
         */
        void w(Type value) {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(4);
            data[3] = value;
        }

        /**
         * @brief Returns the w (fourth) value of the vector
         * @details This is a getter for the w (fourth) value of the vector.
         * @return The w (fourth) value of the vector
         */
        [[nodiscard]] Type getW() const {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(4);
            return data[3];
        }

        /**
         * @brief Sets the w (fourth) value of the vector
         * @details This works as a setter for the w (fourth) value of the vector.
         * @param value The value to set
         */
        void setW(Type value) {
            S_ASSERT_VEC_IS_OF_SIZE_OR_BIGGER(4);
            data[3] = value;
        }

        // =========================================================================================
        // ======================================= Operators =======================================
        // =========================================================================================

        // ================================= Assignment Operators ==================================

        /**
         * @brief Copy assignment operator
         * @details Copies the data from the other vector to this one
         * @param other
         * @return A reference to this vector
         */
        constexpr Vector& operator=(const Vector& other) noexcept {
            VectorAlgorithms::vectorCopy(data, other.data);
            return *this;
        }

        /**
         * @brief Move assignment operator
         * @details Moves the data from the other vector to this one
         * @param other The vector to move from
         * @return A reference to this vector
         */
        constexpr Vector& operator=(Vector&& other) noexcept {
            VectorAlgorithms::vectorMove(data, other.data);
            return *this;
        }

        /**
         * @brief Array assignment operator
         * @details Assigns the values from the array to this vector
         * @param list
         * @return A reference to this vector
         */
        constexpr Vector& operator=(const Type (&list)[N]) noexcept {
            VectorAlgorithms::vectorInitRawArray(data, list);
            return *this;
        }

        /**
         * @brief In-place addition with another vector
         * @details Adds the elements of the given vector to this vector element-wise.
         * @param rhs The vector to add
         * @return A reference to this vector after the operation
         */
        constexpr Vector operator+=(const Vector& rhs) {
            VectorAlgorithms::vectorAdd(this->data, rhs.data, this->data);
            return *this;
        }

        /**
         * @brief In-place addition with a scalar
         * @details Adds the scalar value to each element of this vector.
         * @param scalar The scalar value to add
         * @return A reference to this vector after the operation
         */
        constexpr Vector operator+=(Type scalar) {
            VectorAlgorithms::vectorScalarAdd(this->data, scalar, this->data);
            return *this;
        }

        /**
         * @brief In-place subtraction with another vector
         * @details Subtracts the elements of the given vector from this vector element-wise.
         * @param rhs The vector to subtract
         * @return A reference to this vector after the operation
         */
        constexpr Vector operator-=(const Vector& rhs) {
            VectorAlgorithms::vectorSub(this->data, rhs.data, this->data);
            return *this;
        }

        /**
         * @brief In-place subtraction with a scalar
         * @details Subtracts the scalar value from each element of this vector.
         * @param scalar The scalar value to subtract
         * @return A reference to this vector after the operation
         */
        constexpr Vector operator-=(const Type scalar) {
            VectorAlgorithms::vectorScalarSub(this->data, scalar, this->data);
            return *this;
        }

        /**
         * @brief In-place multiplication with a scalar
         * @details Multiplies each element of this vector by the scalar value.
         * @param scalar The scalar value to multiply with
         * @return A reference to this vector after the operation
         */
        constexpr Vector operator*=(const Type scalar) {
            VectorAlgorithms::vectorScalarMul(this->data, scalar, this->data);
            return *this;
        }

        /**
         * @brief In-place multiplication with another vector
         * @details Multiplies the elements of this vector with the elements of the given vector
         * element-wise.
         * @param rhs The vector to multiply with
         * @return A reference to this vector after the operation
         */
        constexpr Vector operator*=(const Vector& rhs) {
            VectorAlgorithms::vectorMul(this->data, rhs.data, this->data);
            return *this;
        }

        /**
         * @brief In-place division with a scalar
         * @details Divides each element of this vector by the scalar value.
         * @param scalar The scalar value to divide by
         * @return A reference to this vector after the operation
         */
        constexpr Vector operator/=(const Type scalar) {
            VectorAlgorithms::vectorScalarDiv(this->data, scalar, this->data);
            return *this;
        }

        /**
         * @brief In-place division with another vector
         * @details Divides the elements of this vector by the elements of the given vector
         * element-wise.
         * @param rhs The vector to divide by
         * @return A reference to this vector after the operation
         */
        constexpr Vector operator/=(const Vector& rhs) {
            VectorAlgorithms::vectorDiv(this->data, rhs.data, this->data);
            return *this;
        }


        // ================================== Arithmetic Operators =================================

        /**
         * @brief Overloads the '+' operator for scalar addition
         * @details Adds a scalar to each component of the vector
         * @param scalar The scalar value to add
         * @return A new vector containing the result
         */
        constexpr Vector operator+(const Type& scalar) const {
            Vector result;
            VectorAlgorithms::vectorScalarAdd(this->data, scalar, result.data);
            return result;
        }

        /**
         * @brief Overloads the '+' operator for vector addition
         * @details Adds the corresponding data of two vectors
         * @param rhs The right-hand-side vector to add
         * @return A new vector containing the result
         */
        constexpr Vector operator+(const Vector& rhs) const {
            Vector result;
            VectorAlgorithms::vectorAdd(this->data, rhs.data, result.data);
            return result;
        }

        /**
         * @brief Overloads the unary '-' operator for negation
         * @details Negates each component of the vector
         * @return A new vector containing the result
         */
        constexpr Vector operator-() const {
            Vector result;
            VectorAlgorithms::vectorNegate(this->data, result.data);
            return result;
        }

        /**
         * @brief Overloads the '-' operator for scalar subtraction
         * @details Subtracts a scalar from each component of the vector
         * @param scalar The scalar value to subtract
         * @return A new vector containing the result
         */
        constexpr Vector operator-(const Type& scalar) const {
            Vector result;
            VectorAlgorithms::vectorScalarSub(this->data, scalar, result.data);
            return result;
        }

        /**
         * @brief Overloads the '-' operator for vector subtraction
         * @details Subtracts the corresponding data of two vectors
         * @param rhs The right-hand-side vector to subtract
         * @return A new vector containing the result
         */
        constexpr Vector operator-(const Vector& rhs) const {
            Vector result;
            VectorAlgorithms::vectorSub(this->data, rhs.data, result.data);
            return result;
        }

        /**
         * @brief Overloads the '/' operator for scalar division
         * @details Divides each component of the vector by a scalar
         * @param scalar The scalar value to divide by
         * @return A new vector containing the result
         */
        constexpr Vector operator/(Type scalar) const {
            Vector result;
            VectorAlgorithms::vectorScalarDiv(this->data, scalar, result.data);
            return result;
        }

        /**
         * @brief Overloads the '/' operator for vector division
         * @details Divides the corresponding data of two vectors
         * @param rhs The right-hand-side vector to divide by
         * @return A new vector containing the result
         */
        constexpr Vector operator/(const Vector& rhs) const {
            Vector result;
            VectorAlgorithms::vectorDiv(this->data, rhs.data, result.data);
            return result;
        }

        /**
         * @brief Overloads the '*' operator for scalar multiplication
         * @details Multiplies each component of the vector by a scalar
         * @param scalar The scalar value to multiply by
         * @return A new vector containing the result
         */
        constexpr Vector operator*(Type scalar) const {
            Vector result;
            VectorAlgorithms::vectorScalarMul(this->data, scalar, result.data);
            return result;
        }

        /**
         * @brief Overloads the '*' operator for vector multiplication
         * @details Multiplies the corresponding data of two vectors
         * @param rhs The right-hand-side vector to multiply by
         * @return A new vector containing the result
         */
        constexpr Vector operator*(const Vector& rhs) const {
            Vector result;
            VectorAlgorithms::vectorMul(this->data, rhs.data, result.data);
            return result;
        }


        // ==================================== Comparison Operators ================================

        /**
         * @brief Equality operator for vectors
         * @details Compares this vector to another, allowing for different underlying types.
         * Handles floating-point comparison via 'eq' function.
         * @param rhs Vector to compare against
         * @return True if the vectors are equal, false otherwise
         */
        template <typename OtherType>
        constexpr bool operator==(const Vector<OtherType, N>& rhs) const {
            return VectorAlgorithms::eq(this->data, rhs.data);
        }

        /**
         * @brief Inequality operator for vectors
         * @details Utilizes the equality operator for the actual comparison
         * @param rhs Vector to compare against
         * @return True if the vectors are not equal, false otherwise
         */
        constexpr bool operator!=(const Vector& rhs) const {
            return VectorAlgorithms::neq(this->data, rhs.data);
        }

        /**
         * @brief Less than operator for vectors
         * @details Compares vectors using their lengths
         * @param rhs Vector to compare against
         * @return True if this vector's length is less than rhs's length, false otherwise
         */
        constexpr bool operator<(const Vector& rhs) const {
            return VectorAlgorithms::lt(this->data, rhs.data);
        }

        /**
         * @brief Greater than operator for vectors
         * @details Derived from the less-than-or-equal operator
         * @param rhs Vector to compare against
         * @return True if this vector is greater than rhs, false otherwise
         */
        constexpr bool operator>(const Vector& rhs) const {
            return !(this->operator<=(rhs));
        }

        /**
         * @brief Less than or equal to operator for vectors
         * @details Derived from the greater-than operator
         * @param rhs Vector to compare against
         * @return True if this vector is less than or equal to rhs, false otherwise
         */
        constexpr bool operator<=(const Vector& rhs) const {
            return *this < rhs || *this == rhs;
        }

        /**
         * @brief Greater than or equal to operator for vectors
         * @details Derived from the less-than operator
         * @param rhs Vector to compare against
         * @return True if this vector is greater than or equal to rhs, false otherwise
         */
        constexpr bool operator>=(const Vector& rhs) const {
            return !(*this < rhs);
        }


        // =========================================================================================
        // ==================================== Vector Methods =====================================
        // =========================================================================================

        /**
         * @brief Converts the vector to a single column matrix
         * @return The matrix representation of the vector
         */
        [[nodiscard]] MatrixData<Type, N, 1> toMatrix() const {
            MatrixData<Type, N, 1> result;
            VectorMixedAlgorithms::vectorToMatrix(this->data, result);
            return result;
        }

        /**
         * @brief Checks if the vector is a zero vector
         * @return True if the vector is a zero vector, false otherwise
         */
        [[nodiscard]] bool isZero() const {
            return VectorAlgorithms::isZero(this->data);
        }

        /**
         * @brief Gives the distance between two vectors
         * @param other The vector to calculate the distance to
         * @return The distance between the two vectors
         */
        [[nodiscard]] Type distance(const Vector& other) const {
            return VectorAlgorithms::distance(this->data, other.data);
        }

        /**
         * @brief Gives the squared distance between two vectors
         * @param other The vector to calculate the squared distance to
         * @return The squared distance between the two vectors
         */
        [[nodiscard]] Type distanceSquared(const Vector& other) const {
            return VectorAlgorithms::distanceSquared(this->data, other.data);
        }

        /**
         * @brief Gives the rads between two vectors
         * @param other The vector to calculate the angle to
         * @return The angle between the two vectors
         */
        [[nodiscard]] Type angle(const Vector& other) const {
            return VectorAlgorithms::angle(this->data, other.data);
        }

        /**
         * @brief Gives the angle between two vectors in degrees
         * @param other The vector to calculate the angle to
         * @return The angle between the two vectors in degrees
         */
        [[nodiscard]] Type angleDegrees(const Vector& other) const {
            return Math::degrees(VectorAlgorithms::angle(this->data, other.data));
        }

        /**
         * @brief Clamps the vector to a given range
         * @param min The minimum value of the range
         * @param max The maximum value of the range
         * @return The clamped vector
         */
        [[nodiscard]] Vector clamp(Type min, Type max) const {
            Vector result;
            VectorAlgorithms::clampWithValues(this->data, min, max, result.data);
            return result;
        }

        /**
         * @brief Clamps the vector to a given range
         * @param min The minimum values of the range
         * @param max The maximum values of the range
         * @return The clamped vector
         */
        [[nodiscard]] Vector clamp(const Vector& min, const Vector& max) const {
            Vector result;
            VectorAlgorithms::clampWithVectors(this->data, min.data, max.data, result.data);
            return result;
        }

        /**
         * @brief Returns the sum of all the elements in the vector
         * @return The sum of all the elements in the vector
         */
        [[nodiscard]] Type sum() const {
            return VectorAlgorithms::sum(this->data);
        }

        /**
         * @brief Converts the vector to radians
         * @return The vector in radians
         */
        [[nodiscard]] Vector toRads() const {
            Vector result;
            for (size_t i = 0; i < N; i++) {
                result[i] = Math::radians(data[i]);
            }
            return result;
        }

        /**
         * @brief Converts the vector to degrees
         * @return The vector in degrees
         */
        [[nodiscard]] Vector toDegrees() const {
            Vector result;
            for (size_t i = 0; i < N; i++) {
                result[i] = Math::degrees(data[i]);
            }
            return result;
        }

        /**
         * @brief Returns the absolute value of the vector
         * @details The absolute value of a vector is a vector where each component is the absolute
         * @return The absolute value of the vector
         */
        [[nodiscard]] Vector abs() const noexcept {
            Vector result;
            VectorAlgorithms::abs(this->data, result.data);
            return result;
        }

        /**
         * @brief Returns the mod of the vector given a divisor
         * @details The mod of a vector is a vector where each component is the remainder of the
         * division of the component by the divisor.
         * @param divisor The divisor to use
         * @return The mod of the vector
         */
        [[nodiscard]] Vector mod(Type divisor) const {
            Vector result;
            VectorAlgorithms::modWithValue(this->data, divisor, result.data);
            return result;
        }

        /**
         * @brief Returns the mod of the vector given a vector of divisors
         * @details The mod of a vector is a vector where each component is the remainder of the
         * division of the component by the corresponding divisor.
         * @param divisors The vector of divisors to use
         * @return The mod of the vector
         */
        [[nodiscard]] Vector mod(Vector divisors) const {
            Vector result;
            VectorAlgorithms::modWithVector(this->data, divisors.data, result.data);
            return result;
        }

        /**
         * @brief Calculates the linear interpolation between two vectors using a factor
         * @details The linear interpolation between two vectors is a vector where each component
         * is the linear interpolation of the corresponding components of the two vectors.
         * @param other The other vector to interpolate with
         * @param factor The factor to use for the interpolation
         * @return The linear interpolation between the two vectors
         */
        [[nodiscard]] Vector lerp(const Vector& other, Type factor) const {
            Vector result;
            VectorAlgorithms::lerp(this->data, other.data, factor, result.data);
            return result;
        }

        /**
         * @brief Swaps the data of this vector with another vector
         * @param other The other vector to swap with
         */
        void swap(Vector& other) noexcept {
            std::swap(data, other.data);
        }

        /**
         * @brief Returns the dot product of two vectors
         * @details The dot product of two vectors is a scalar value that represents the
         * projection of one vector onto the other.
         * @param rhs The vector to calculate the dot product with
         * @return The dot product of the two vectors
         */
        [[nodiscard]] Type dot(const Vector& rhs) const {
            return VectorAlgorithms::dotProduct(this->data, rhs.data);
        }

        /**
         * @brief Returns the length of the vector
         * @details The length of a vector is the square root of the sum of the squares of its
         * components.
         * @return The length of the vector
         */
        [[nodiscard]] Type length() const {
            return VectorAlgorithms::length(this->data);
        }

        /**
         * @brief Returns the length of the vector
         * @details The length of a vector is the square root of the sum of the squares of its
         * components.
         * @return The length of the vector
         */
        [[nodiscard]] Type magnitude() const {
            return VectorAlgorithms::length(this->data);
        }

        /**
         * @brief Normalizes the vector
         * @details Normalizing a vector means dividing each component of the vector by the length
         * of the vector. Modifies the vector in place.
         * @return A reference to this vector after the operation
         */
        Vector& normalize() {
            VectorAlgorithms::normalize(this->data, data);
            return *this;
        }

        /**
         * @brief Normalizes the vector
         * @details Normalizing a vector means dividing each component of the vector by the length
         * of the vector.
         * @return The normalized vector
         */
        [[nodiscard]] Vector normalize() const {
            Vector result;
            VectorAlgorithms::normalize(this->data, result.data);
            return result;
        }

        /**
         * @brief checks if the vector is normalized
         * @details A normalized vector is a vector whose length is equal to 1.
         * @return True if the vector is normalized, false otherwise
         */
        [[nodiscard]] bool isNormalized() const {
            return VectorAlgorithms::isNormalized(this->data);
        }

        /**
         * @brief Returns the squared length of the vector
         * @details The squared length of a vector is the sum of the squares of its components.
         * @return The squared length of the vector
         */
        [[nodiscard]] Type lengthSquared() const {
            return VectorAlgorithms::lengthSquared(this->data);
        }

        /**
         * @brief Checks if the vector is homogeneous
         * @details A homogeneous vector is a vector whose last component is not equal to zero.
         * @return True if the vector is homogeneous, false otherwise
         */
        [[nodiscard]] Type isHomogeneous() const {
            return VectorAlgorithms::isHomogeneous(this->data);
        }

        /**
         * @brief Homogenizes the vector
         * @details Homogenizing a vector means adding a 1 as the last component of the vector.
         * @return The homogenized vector (a vector with one more component)
         */
        [[nodiscard]] Vector<Type, N + 1> homogenize() const {
            Vector<Type, N + 1> result;
            VectorAlgorithms::homogenize(this->data, result.data);
            return result;
        }

        /**
         * @brief Dehomogenizes the vector
         * @details Dehomogenizing a vector means dividing each component of the vector by the last
         * component of the vector.
         * @return The dehomogenized vector (a vector with one less component)
         */
        [[nodiscard]] Vector<Type, N - 1> dehomogenize() const {
            Vector<Type, N - 1> result;
            VectorAlgorithms::dehomogenize(this->data, result.data);
            return result;
        }


        /**
         * @brief Projects the vector onto another vector
         * @details The projection of a vector onto another vector is the vector that results from
         * projecting the first vector onto the second vector.
         * @param other The vector to project onto
         * @return The projected vector
         */
        [[nodiscard]] Vector project(const Vector& other) const {
            Vector result;
            VectorAlgorithms::project(this->data, other.data, result.data);
            return result;
        }

        /**
         * @brief Returs the forward vector of a rotation
         * @details The forward vector of a rotation is the vector that points in the direction of
         * an object that has been rotated by the given rotation.
         * @param rotation The rotation to get the forward vector from
         * @return The forward vector of the rotation
         */
        [[nodiscard]] Vector& makeForward(const Vector& rotation) {
            VectorAlgorithms::makeForwardVector(rotation.data, this->data);
            return *this;
        }

        /**
         * @brief Returns the right vector of a rotation
         * @details The right vector of a rotation is the vector that points to the right of an
         * object that has been rotated by the given rotation.
         * @param rotation The rotation to get the right vector from
         * @return The right vector of the rotation
         */
        [[nodiscard]] Vector& makeRight(const Vector& rotation) {
            VectorAlgorithms::makeRightVector(rotation.data, this->data);
            return *this;
        }

        /**
         * @brief Returns the up vector of a rotation
         * @details The up vector of a rotation is the vector that points up from an object that has
         * been rotated by the given rotation.
         * @param rotation The rotation to get the up vector from
         * @return The up vector of the rotation
         */
        [[nodiscard]] Vector& makeUp(const Vector& rotation) {
            VectorAlgorithms::makeUpVector(rotation.data, this->data);
            return *this;
        }

        /**
         * @brief Rotates the vector by a given rotation
         * @details Rotating a vector means applying a rotation to the vector.
         * @param rotation The rotation to apply
         * @return A reference to this vector after the operation
         */
        [[nodiscard]] Vector<Type, 3>& rotate(const Vector<Type, 3>& rotation) {
            VectorAlgorithms::rotateVector3D(this->data, rotation.data, this->data);
            return *this;
        }

        /**
         * @brief Rotates the vector by a given rotation
         * @details Rotating a vector means applying a rotation to the vector.
         * @param rotation The rotation to apply
         * @return The rotated vector
         */
        [[nodiscard]] Vector<Type, 3> rotate(const Vector<Type, 3>& rotation) const {
            Vector<Type, 3> result;
            VectorAlgorithms::rotateVector3D(this->data, rotation.data, result.data);
            return result;
        }


        /**
         * @brief Reflects the vector by a given normal, modifying the current vector
         * @details Reflecting a vector means reflecting the vector by a given normal.
         * @param normal The normal to reflect the vector by
         * @return A reference to this vector after the operation
         */
        [[nodiscard]] Vector& reflect(const Vector& normal) {
            VectorAlgorithms::reflect(this->data, normal.data, this->data);
            return *this;
        }

        /**
         * @brief Reflects the vector by a given normal
         * @details Reflecting a vector means reflecting the vector by a given normal.
         * @param normal The normal to reflect the vector by
         * @return The reflected vector
         */
        [[nodiscard]] Vector reflect(const Vector& normal) const {
            Vector result;
            VectorAlgorithms::reflect(this->data, normal.data, result.data);
            return result;
        }

        /**
         * @brief Converts the vector to euler angles
         * @details Takes the vector as a direction vector, receives a up vector and returns the
         * euler angles that represents the rotation of an object that needs to be rotated to look
         * in the direction of the direction vector.
         * Modifies the current vector.
         * @param up The up vector
         * @return A reference to this vector after the operation
         */
        [[nodiscard]] Vector& vectorToEulerRotation(const Vector& up) {
            VectorAlgorithms::vectorToEuler(this->data, up.data, this->data);
            return *this;
        }
        /**
         * @brief Converts the vector to euler angles
         * @details Takes the vector as a direction vector, receives a up vector and returns the
         * euler angles that represents the rotation of an object that needs to be rotated to look
         * in the direction of the direction vector.
         * @param up The up vector
         * @return The euler angles that represent the rotation
         */
        [[nodiscard]] Vector vectorToEulerRotation(const Vector& up) const {
            Vector result;
            VectorAlgorithms::vectorToEuler(this->data, up.data, result.data);
            return result;
        }

        /**
         * @brief Returns the size of the vector
         * @return The size of the vector
         */
        [[nodiscard]] int size() const {
            return N;
        }

        /**
         * @brief Checks if the vector is parallel to another vector
         * @details Two vectors are parallel if they have the same direction or are scalar multiples
         * of each other.
         * @param other The vector to compare against
         * @return True if the vectors are parallel, false otherwise
         */
        [[nodiscard]] bool isParallel(const Vector& other) const {
            return VectorAlgorithms::areParallel(this->data, other.data);
        }

        /**
         * @brief Checks if the vector is orthogonal to another vector
         * @details Two vectors are orthogonal if their dot product is zero.
         * @param other The vector to compare against
         * @return True if the vectors are orthogonal, false otherwise
         */
        [[nodiscard]] bool isOrthogonal(const Vector& other) const {
            return VectorAlgorithms::areOrthogonal(this->data, other.data);
        }

        /**
         * @brief Get an orthogonal (or perpendicular) vector.
         * @return A new vector that is orthogonal to this one. If the vector has more than 2
         * dimensions, the axis of orthogonality is chosen arbitrarily.
         */
        [[nodiscard]] Vector getOrthogonal() const {
            Vector result;
            VectorAlgorithms::getOrthogonal(this->data, result.data);
            return result;
        }


        /**
         * @brief Checks if the current vector is a scalar multiple of another vector.
         *
         * @param other The vector to compare against.
         * @return true If the current vector is a scalar multiple of the other vector.
         * @return false Otherwise.
         */
        bool isMultipleOf(const Vector& other) const {
            return VectorAlgorithms::isMultipleOf(this->data, other.data);
        }

        /**
         * @brief Checks if the current vector is a scalar multiple of some vectors
         * @param points The vectors to compare against
         * @return true If the current vector is a scalar multiple of all the other vectors.
         */
        bool isCollinear(const std::vector<Vector<Type, 3>>& points) const {
            // Create a vector of pointers to the data of each point
            std::vector<const VectorData<Type, 3>*> pointData;

            for (const auto& point : points) {
                pointData.push_back(&point.data);
            }

            // Call the areCollinear function with the reference point's data and the vector of data pointers
            return VectorMixedAlgorithms::areCollinear(this->data, pointData);
        }


        /**
         * @brief Calculates the cross product of two vectors
         * @details The cross product is a vector that is perpendicular to the
         * plane formed by the two vectors.
         * Only works for 3 dimensional vectors.
         * @param other
         * @return
         */
        [[nodiscard]] Vector<Type, 3> cross(const Vector<Type, 3>& other) const {
            Vector<Type, 3> result;
            VectorAlgorithms::crossProduct(this->data, other.data, result.data);
            return result;
        }

        /**
         * @brief Converts the vector to string
         * @return The string representation of the vector
         */
        [[nodiscard]] std::string toString() const {
            return VectorAlgorithms::toString(this->data);
        }

        /**
         * @brief Data of the vector
         * @see VectorData
         */
        VectorData<Type, N> data;
    };
}
#pragma pack(pop)  // Restore the previous alignment from the stack

template <typename Type, size_t N>
using VectorT = GLESC::Math::Vector<Type, N>;

using Vec2 = VectorT<float, 2>;
using Vec3 = VectorT<float, 3>;
using Vec4 = VectorT<float, 4>;

using Vec2B = VectorT<bool, 2>;
using Vec3B = VectorT<bool, 3>;
using Vec4B = VectorT<bool, 4>;

using Vec2UI = VectorT<unsigned int, 2>;
using Vec3UI = VectorT<unsigned int, 3>;
using Vec4UI = VectorT<unsigned int, 4>;

using Vec2I = VectorT<int, 2>;
using Vec3I = VectorT<int, 3>;
using Vec4I = VectorT<int, 4>;

using Vec2UL = VectorT<unsigned long, 2>;
using Vec3UL = VectorT<unsigned long, 3>;
using Vec4UL = VectorT<unsigned long, 4>;

using Vec2L = VectorT<long, 2>;
using Vec3L = VectorT<long, 3>;
using Vec4L = VectorT<long, 4>;

using Vec2F = VectorT<float, 2>;
using Vec3F = VectorT<float, 3>;
using Vec4F = VectorT<float, 4>;

using Vec2D = VectorT<double, 2>;
using Vec3D = VectorT<double, 3>;
using Vec4D = VectorT<double, 4>;

template <typename T>
struct is_vector : std::false_type {
};

/**
 * @brief Specialization of the is_vector type trait for the Vector class
 * @details This specialization makes the is_vector type trait return true for the Vector class.
 * It is used to check if a type is a vector.
 */
template <typename T, std::size_t N>
struct is_vector<GLESC::Math::Vector<T, N>> : std::true_type {
};

/**
 * @brief Helper variable template for the is_vector type trait
 * @details This variable template is used to check if a type is a vector.
 * It is used to check if a type is a vector.
 */
template <typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

/**
 * @brief Specialization of the std::hash type trait for the Vector class
 * @details This specialization makes the std::hash type trait work with the Vector class.
 * It is used to hash vectors.
 */
template <typename T, size_t U>
struct std::hash<VectorT<T, U>> {
    std::size_t operator()(const VectorT<T, U>& vec) const {
        std::hash<T> hasher;
        std::size_t seed = 0;
        for (size_t i = 0; i < U; ++i) {
            seed ^= hasher(vec[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
