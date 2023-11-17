/******************************************************************************
 * @file   Matrix.h
 * @author Valentin Dumitru
 * @date   2023-10-03
 * @brief  @todo Add description of this file if needed
 *
 * Copyright (c) 2023 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 ******************************************************************************/
#pragma once

#include <cmath>
#include <cassert>
#include <memory>
#include <algorithm>
#include "Vec.h"
#include "engine/core/exceptions/core/math/MathException.h"
#include "engine/core/math/asserts/MatrixAsserts.h"
#include "engine/core/logger/Logger.h"
#include "engine/core/debugger/Stacktrace.h"
#include "engine/core/math/MatrixAlgorithms.h"

namespace GLESC::Math {
    template<typename Type, size_t N, size_t M>
    class Matrix {
    public:
        // =========================================================================================
        // ======================================= Constructors ====================================
        // =========================================================================================
        
        Matrix() {
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; ++k) {
                    data[i][k] = Type();
                }
            }
        }
        
        explicit Matrix(Type diagonal) {
            for (size_t i = 0; i < N; ++i) {
                data[i][i] = diagonal;
            }
        }
        
        /**
         * @brief Constructor array list
         * @param data
         */
        Matrix(Type (&data)[N][M]) {
            std::copy(std::begin(data), std::end(data), std::begin(this->data));
        }
        
        /**
         * @brief Copy constructor
         * @param other
         */
        Matrix(const Matrix<Type, N, M> &other) {
            std::copy(std::begin(other.data), std::end(other.data), std::begin(data));
        }
        
        /**
         * @brief Move constructor
         * @param list
         */
        Matrix(Matrix<Type, N, M> &&other) noexcept {
            std::move(std::begin(other.data), std::end(other.data), std::begin(data));
        }
        
        /**
         * @brief Constructor from initializer list
         * @param list
         */
        Matrix(std::initializer_list<std::initializer_list<Type>> list) {
            // TODO: Use custom assert, for some reason its bugged
            assert(list.size() == N && "Matrix rows size is incorrect");
            size_t i = 0;
            for (const auto &row : list) {
                size_t k = 0;
                for (const auto &col : row) {
                    data[i][k] = col;
                    ++k;
                }
                ++i;
            }
        }
        
        // =========================================================================================
        // ========================================= Operators =====================================
        // =========================================================================================
        
        
        // ================================== Assignment Operators =================================
        
        /**
         * @brief Copy assignment operator.
         * @param other The matrix to copy from.
         * @return Reference to the modified instance.
         */
        Matrix<Type, N, M> &operator=(const Matrix<Type, N, M> &other) noexcept {
            if (this == &other) {
                return *this;
            }
            std::copy(std::begin(other.data), std::end(other.data), std::begin(data));
            return *this;
        }
        
        /**
         * @brief Move assignment operator.
         * @param rhs The matrix to move from.
         * @return Reference to the modified instance.
         */
        Matrix<Type, N, M> &operator=(Matrix<Type, N, M> &&rhs) noexcept {
            if (this == &rhs) {
                return *this;
            }
            std::move(std::begin(rhs.data), std::end(rhs.data), std::begin(data));
            return *this;
        }
        
        /**
         * @brief Assignment operator from a 2D array.
         * @param data 2D array to assign from.
         * @return Reference to the modified instance.
         */
        Matrix<Type, N, M> &operator=(Type (&data)[N][M]) {
            std::copy(std::begin(data), std::end(data), std::begin(this->data));
            return *this;
        }
        
        /**
         * @brief Assignment operator from an initializer list of initializer lists.
         * @param list Initializer list of initializer lists to assign from.
         * @return Reference to the modified instance.
         */
        Matrix<Type, N, M> &operator=(std::initializer_list<std::initializer_list<Type>> list) {
            size_t i = 0;
            for (const auto &row : list) {
                size_t k = 0;
                for (const auto &col : row) {
                    data[i][k] = col;
                    ++k;
                }
                ++i;
            }
            return *this;
        }
        
        /**
         * @brief In-place element-wise multiplication with another matrix.
         * @param rhs The matrix to multiply with.
         * @return Reference to the modified instance.
         */
        template<size_t X>
        Matrix<Type, N, M> &operator*=(const Matrix<Type, M, X> &rhs) {
            S_ASSERT_MAT_IS_SQUARE(M, X);
            Matrix<Type, N, X> result;
            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < X; ++j) {
                    result[i][j] = 0;
                    for (size_t k = 0; k < M; ++k) {
                        result[i][j] += data[i][k] * rhs[k][j];
                    }
                }
            }
            *this = std::move(result);
            return *this;
        }
        
        /**
         * @brief In-place scalar multiplication.
         * @param scalar The scalar to multiply with.
         * @return Reference to the modified instance.
         */
        Matrix<Type, N, M> &operator*=(Type scalar) {
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; ++k) {
                    data[i][k] *= scalar;
                }
            }
            return *this;
        }
        
        /**
         * @brief In-place scalar addition.
         * @param rhs The scalar to add.
         * @return Reference to the modified instance.
         */
        Matrix<Type, N, M> &operator+=(Type rhs) {
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; k++) {
                    data[i][k] += rhs;
                }
            }
            return *this;
        }
        
        /**
         * @brief In-place element-wise addition with another matrix.
         * @param rhs The matrix to add with.
         * @return Reference to the modified instance.
         */
        Matrix<Type, N, M> &operator+=(const Matrix<Type, N, M> &rhs) {
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; k++) {
                    data[i][k] += rhs.data[i][k];
                }
            }
            return *this;
        }
        
        /**
         * @brief In-place element-wise subtraction with another matrix.
         * @param rhs The matrix to subtract with.
         * @return Reference to the modified instance.
         */
        Matrix<Type, N, M> &operator-=(const Matrix<Type, N, M> &rhs) {
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; k++) {
                    data[i][k] -= rhs.data[i][k];
                }
            }
            return *this;
        }
        
        /**
         * @brief In-place scalar subtraction.
         * @param rhs The scalar to subtract.
         * @return Reference to the modified instance.
         */
        Matrix<Type, N, M> &operator-=(Type rhs) {
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; k++) {
                    data[i][k] -= rhs;
                }
            }
            return *this;
        }
        
        /**
         * @brief In-place scalar division.
         * @param scalar The scalar to divide by.
         * @return Reference to the modified instance.
         * @throws MathException if division by zero
         */
        Matrix<Type, N, M> &operator/=(Type scalar) {
            if (eq(scalar, Type(0))) {
                throw MathException("Division by zero");
            }
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; k++) {
                    data[i][k] /= scalar;
                }
            }
            return *this;
        }
        
        /**
         * @brief In-place division by another matrix.
         * @param rhs The matrix to divide by.
         * @return Reference to the modified instance.
         * @throws MathException if division by zero
         */
        Matrix<Type, N, M> &operator/=(const Matrix<Type, N, M> &rhs) {
            if (eq(rhs.determinant(), Type(0))) {
                throw MathException("Division by zero");
            }
            *this *= rhs.inverse();
            return *this;
        }
        
        // ================================== Arithmetic Operators =================================
        
        [[nodiscard]] Matrix<Type, N, M> operator+(const Matrix<Type, N, M> &rhs) const {
            Matrix<Type, N, M> result;
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; ++k) {
                    result.data[i][k] = data[i][k] + rhs.data[i][k];
                }
            }
            return result;
        }
        
        [[nodiscard]] Matrix<Type, N, M> operator+(Type scalar) const {
            Matrix<Type, N, M> result;
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; k++) {
                    result.data[i][k] = data[i][k] + scalar;
                }
            }
            return result;
        }
        
        [[nodiscard]] Matrix<Type, N, M> operator-(const Matrix<Type, N, M> &rhs) const {
            Matrix<Type, N, M> result;
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; ++k) {
                    result.data[i][k] = data[i][k] - rhs.data[i][k];
                }
            }
            return result;
        }
        
        [[nodiscard]] Matrix<Type, N, M> operator-(Type scalar) const {
            Matrix<Type, N, M> result;
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; k++) {
                    result.data[i][k] = data[i][k] - scalar;
                }
            }
            return result;
        }
        
        [[nodiscard]] Matrix<Type, N, M> operator-() const {
            Matrix<Type, N, M> result;
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; k++) {
                    result.data[i][k] = -data[i][k];
                }
            }
            return result;
        }
        
        /**
         * @brief Multiplying Matrix by Matrix
         * @details A matrix NxM multiplied by a matrix MxX results in a matrix N x X
         * For example:
         * | 1 2 3 |   | 7  8  |   | 58  64  |
         * | 4 5 6 | * | 9  10 | = | 139 154 |
         *             | 11 12 |
         * 2x3      *  3x2       = 2x2
         * @tparam X
         * @param other
         * @return
         */
        template<size_t X>
        [[nodiscard]] Matrix<Type, N, X> operator*(const Matrix<Type, M, X> &other) const {
            Matrix<Type, N, X> result;
            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < X; ++j) {
                    for (size_t k = 0; k < M; ++k) {
                        result[i][j] += (*this).get(i, k) * other.get(k, j);
                    }
                }
            }
            return result;
        }
        
        /**
         * @brief Multiplying Matrix by Vector
         * @details A matrix NxM multiplied by a vector Mx1 results in a vector N
         * (which acts as a matrix Nx1)
         * For example:
         * | 1 2 3 |   | 7 |   | 58 |
         * | 4 5 6 | * | 8 | = | 139 |
         *             | 9 |
         * 2x3      *  3x1       = 2x1
         * @tparam X
         * @param other
         * @return
         */
        [[nodiscard]] Vector<Type, N> operator*(const Vector<Type, M> &other) const {
            Vector<Type, N> result;
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; ++k) {
                    result[i] += data[i][k] * other[k];
                }
            }
            return result;
        }
        
        
        [[nodiscard]] Matrix<Type, N, M> operator*(Type scalar) const {
            Matrix<Type, N, M> result;
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; ++k) {
                    result.data[i][k] = data[i][k] * scalar;
                }
            }
            return result;
        }
        
        
        [[nodiscard]] Matrix<Type, N, M> operator/(Type scalar) const {
            if (eq(scalar, Type()))
                throw MathException("Division by zero");
            Matrix<Type, N, M> result;
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; ++k) {
                    result.data[i][k] = data[i][k] / scalar;
                }
            }
            return result;
        }
        
        /**
         * @brief Matrix division by matrix
         * @details The matrix division by matrix is defined as the multiplication of the first
         * matrix by the inverse of the second matrix
         * @param rhs
         * @return
         */
        [[nodiscard]] Matrix<Type, N, M> operator/(const Matrix<Type, N, M> &rhs) const {
            Matrix<Type, N, M> result;
            if (eq(rhs.determinant(), Type()))
                throw MathException("Division by zero");
            result = *this * rhs.inverse();
            return result;
        }
        
        // ==================================== Access Operators ===================================
        
        [[nodiscard]] const Vector<Type, M> &operator[](size_t index) const {
            return data[index];
        }
        
        [[nodiscard]] Vector<Type, M> &operator[](size_t index) {
            return data[index];
        }
        
        [[nodiscard]] const Type &get(size_t i, size_t j) const {
            return data[i].get(j);
        }
        
        [[nodiscard]] size_t rows() const {
            return N;
        }
        
        [[nodiscard]] size_t cols() const {
            return M;
        }
        
        // ================================== Comparison Operators =================================
        
        [[nodiscard]] bool operator==(const Matrix<Type, N, M> &rhs) const {
            for (size_t i = 0; i < N; ++i) {
                for (size_t k = 0; k < M; ++k) {
                    if (!eq(data[i][k], rhs.data[i][k])) {
                        return false;
                    }
                }
            }
            return true;
        }
        
        [[nodiscard]] bool operator!=(const Matrix<Type, N, M> &rhs) const {
            return !(*this == rhs);
        }
        
        // =========================================================================================
        // =================================== Matrix Functions ====================================
        // =========================================================================================
        
        [[nodiscard]] Matrix<Type, M, N> transpose() const {
            Matrix<Type, M, N> result;
            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < M; ++j) {
                    result[j][i] = data[i][j];
                }
            }
            return result;
        }
        
        [[nodiscard]]Type determinant() const {
            S_ASSERT_MAT_IS_SQUARE(N, M);
            if constexpr (N == 2 && M == 2) {
                return data[0][0] * data[1][1] - data[0][1] * data[1][0];
            } else if constexpr (N == 3 && M == 3) {
                return data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) -
                       data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) +
                       data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
            } else if constexpr (N == 4 && M == 4) {
                Type det;
                det = data[0][0] *
                      (data[1][1] * (data[2][2] * data[3][3] - data[2][3] * data[3][2]) -
                       data[1][2] * (data[2][1] * data[3][3] - data[2][3] * data[3][1]) +
                       data[1][3] * (data[2][1] * data[3][2] - data[2][2] * data[3][1]));
                
                det -= data[0][1] *
                       (data[1][0] * (data[2][2] * data[3][3] - data[2][3] * data[3][2]) -
                        data[1][2] * (data[2][0] * data[3][3] - data[2][3] * data[3][0]) +
                        data[1][3] * (data[2][0] * data[3][2] - data[2][2] * data[3][0]));
                
                det += data[0][2] *
                       (data[1][0] * (data[2][1] * data[3][3] - data[2][3] * data[3][1]) -
                        data[1][1] * (data[2][0] * data[3][3] - data[2][3] * data[3][0]) +
                        data[1][3] * (data[2][0] * data[3][1] - data[2][1] * data[3][0]));
                
                det -= data[0][3] *
                       (data[1][0] * (data[2][1] * data[3][2] - data[2][2] * data[3][1]) -
                        data[1][1] * (data[2][0] * data[3][2] - data[2][2] * data[3][0]) +
                        data[1][2] * (data[2][0] * data[3][1] - data[2][1] * data[3][0]));
                
                return det;
            } else {
                return MatrixAlgorithms::laplaceExpansionDeterminant(*this);
            }
            
        }
        
        [[nodiscard]] Matrix<Type, N, M> inverse() const {
            S_ASSERT_MAT_IS_SQUARE(N, M);
            Type det = determinant();
            Type inDet = 1.0 / det;
            if (eq(det, 0))
                throw MathException("Division by zero");
            if constexpr (N == 2) {
                Matrix<Type, N, M> inv;
                inv.data[0][0] = data[1][1] * inDet;
                inv.data[0][1] = -data[0][1] * inDet;
                inv.data[1][0] = -data[1][0] * inDet;
                inv.data[1][1] = data[0][0] * inDet;
                return inv;
            } else if constexpr (N == 3) {
                Matrix<Type, N, M> inv;
                inv.data[0][0] = (data[1][1] * data[2][2] - data[1][2] * data[2][1]) * inDet;
                inv.data[0][1] = (data[0][2] * data[2][1] - data[0][1] * data[2][2]) * inDet;
                inv.data[0][2] = (data[0][1] * data[1][2] - data[0][2] * data[1][1]) * inDet;
                inv.data[1][0] = (data[1][2] * data[2][0] - data[1][0] * data[2][2]) * inDet;
                inv.data[1][1] = (data[0][0] * data[2][2] - data[0][2] * data[2][0]) * inDet;
                inv.data[1][2] = (data[0][2] * data[1][0] - data[0][0] * data[1][2]) * inDet;
                inv.data[2][0] = (data[1][0] * data[2][1] - data[1][1] * data[2][0]) * inDet;
                inv.data[2][1] = (data[0][1] * data[2][0] - data[0][0] * data[2][1]) * inDet;
                inv.data[2][2] = (data[0][0] * data[1][1] - data[0][1] * data[1][0]) * inDet;
                return inv;
            } else if constexpr (N == 4) {
                
                Type A2323 = data[2][2] * data[3][3] - data[2][3] * data[3][2];
                Type A1323 = data[2][1] * data[3][3] - data[2][3] * data[3][1];
                Type A1223 = data[2][1] * data[3][2] - data[2][2] * data[3][1];
                Type A0323 = data[2][0] * data[3][3] - data[2][3] * data[3][0];
                Type A0223 = data[2][0] * data[3][2] - data[2][2] * data[3][0];
                Type A0123 = data[2][0] * data[3][1] - data[2][1] * data[3][0];
                Type A2313 = data[1][2] * data[3][3] - data[1][3] * data[3][2];
                Type A1313 = data[1][1] * data[3][3] - data[1][3] * data[3][1];
                Type A1213 = data[1][1] * data[3][2] - data[1][2] * data[3][1];
                Type A2312 = data[1][2] * data[2][3] - data[1][3] * data[2][2];
                Type A1312 = data[1][1] * data[2][3] - data[1][3] * data[2][1];
                Type A1212 = data[1][1] * data[2][2] - data[1][2] * data[2][1];
                Type A0313 = data[1][0] * data[3][3] - data[1][3] * data[3][0];
                Type A0213 = data[1][0] * data[3][2] - data[1][2] * data[3][0];
                Type A0312 = data[1][0] * data[2][3] - data[1][3] * data[2][0];
                Type A0212 = data[1][0] * data[2][2] - data[1][2] * data[2][0];
                Type A0113 = data[1][0] * data[3][1] - data[1][1] * data[3][0];
                Type A0112 = data[1][0] * data[2][1] - data[1][1] * data[2][0];
                
                
                Matrix<Type, N, M> in;
                
                in.data[0][0] =
                        inDet * (data[1][1] * A2323 - data[1][2] * A1323 + data[1][3] * A1223);
                in.data[0][1] =
                        inDet * -(data[0][1] * A2323 - data[0][2] * A1323 + data[0][3] * A1223);
                in.data[0][2] =
                        inDet * (data[0][1] * A2313 - data[0][2] * A1313 + data[0][3] * A1213);
                in.data[0][3] =
                        inDet * -(data[0][1] * A2312 - data[0][2] * A1312 + data[0][3] * A1212);
                in.data[1][0] =
                        inDet * -(data[1][0] * A2323 - data[1][2] * A0323 + data[1][3] * A0223);
                in.data[1][1] =
                        inDet * (data[0][0] * A2323 - data[0][2] * A0323 + data[0][3] * A0223);
                in.data[1][2] =
                        inDet * -(data[0][0] * A2313 - data[0][2] * A0313 + data[0][3] * A0213);
                in.data[1][3] =
                        inDet * (data[0][0] * A2312 - data[0][2] * A0312 + data[0][3] * A0212);
                in.data[2][0] =
                        inDet * (data[1][0] * A1323 - data[1][1] * A0323 + data[1][3] * A0123);
                in.data[2][1] =
                        inDet * -(data[0][0] * A1323 - data[0][1] * A0323 + data[0][3] * A0123);
                in.data[2][2] =
                        inDet * (data[0][0] * A1313 - data[0][1] * A0313 + data[0][3] * A0113);
                in.data[2][3] =
                        inDet * -(data[0][0] * A1312 - data[0][1] * A0312 + data[0][3] * A0112);
                in.data[3][0] =
                        inDet * -(data[1][0] * A1223 - data[1][1] * A0223 + data[1][2] * A0123);
                in.data[3][1] =
                        inDet * (data[0][0] * A1223 - data[0][1] * A0223 + data[0][2] * A0123);
                in.data[3][2] =
                        inDet * -(data[0][0] * A1213 - data[0][1] * A0213 + data[0][2] * A0113);
                in.data[3][3] =
                        inDet * (data[0][0] * A1212 - data[0][1] * A0212 + data[0][2] * A0112);
                
                return in;
            } else {
                return MatrixAlgorithms::gaussianInverse(*this);
            }
            
        }
        
        [[nodiscard]] Matrix<Type, N, M> translate(const Vector<Type, N - 1> &translation) const {
            S_ASSERT_MAT_IS_SQUARE(N, M);
            Matrix<Type, N, M> result(*this);
            for (size_t i = 0; i < N - 1; ++i) {
                result.data[i][M - 1] += translation[i];
            }
            return result;
        }
        
        [[nodiscard]] Matrix<Type, N, M> scale(const Vector<Type, N - 1> &scale) const {
            S_ASSERT_MAT_IS_SQUARE(N, M);
            Matrix<Type, N, M> result(*this);
            for (size_t i = 0; i < N - 1; ++i) {
                result.data[i][i] += scale[i];
            }
            return result;
        }
        
        /**
         * @brief Applies a rotation to the model matrix.
         * @details The
         * @tparam VecType
         * @param degrees
         * @return
         */
        template<typename VecType>
        [[nodiscard]] Matrix<Type, N, M> rotate(const VecType &degrees) const {
            if constexpr (std::is_same_v<VecType, Type> && N == 3 && M == 3) {
                return MatrixAlgorithms::rotate2D(*this, static_cast<Type>(degrees));
                
            } else if constexpr (std::is_same_v<VecType, Vector<Type, 3>> && N == 4 && M == 4) {
                return MatrixAlgorithms::rotate3D(*this, static_cast<Vector<Type, 3>>(degrees));
            } else {
                static_assert(N != 3 && M != 3 || N != 4 && M != 4,
                              "Rotation is only supported for 2D and 3D matrices");
            }
        }
        
        
        [[nodiscard]] Matrix<Type, 3, 3> lookAt(const Vector<Type, 2> &target) const {
            return MatrixAlgorithms::lookAt2D(*this, target);
        }
        
        [[nodiscard]] Matrix<Type, 4, 4>
        lookAt(const Vector<Type, 3> &target, const Vector<Type, 3> &up) const {
            return MatrixAlgorithms::lookAt3D(*this, target, up);
        }
        
        
        [[nodiscard]] std::string toString() const {
            std::string result;
            for (size_t i = 0; i < N; ++i) {
                result += "[";
                for (size_t j = 0; j < M; ++j) {
                    result += std::to_string(data[i][j]);
                    if (j != M - 1) {
                        result += ", ";
                    }
                }
                result += "]\n";
            }
            return result;
        }
    
    protected:
        /**
         * @brief Matrix data
         * @details Matrix data is stored in a vector of vectors
         * M is the number of columns or the width of the matrix
         * N is the number of rows (or vertices) or the height of the matrix
         */
        Vector<Type, M> data[N];
    }; // class Matrix
} // namespace GLESC::Math
using Mat2 = GLESC::Math::Matrix<float, 2, 2>;
using Mat3 = GLESC::Math::Matrix<float, 3, 3>;
using Mat4 = GLESC::Math::Matrix<float, 4, 4>;

using Mat2D = GLESC::Math::Matrix<double, 2, 2>;
using Mat3D = GLESC::Math::Matrix<double, 3, 3>;
using Mat4D = GLESC::Math::Matrix<double, 4, 4>;

using Mat2I = GLESC::Math::Matrix<int, 2, 2>;
using Mat3I = GLESC::Math::Matrix<int, 3, 3>;
using Mat4I = GLESC::Math::Matrix<int, 4, 4>;

using Mat2Ui = GLESC::Math::Matrix<unsigned int, 2, 2>;
using Mat3Ui = GLESC::Math::Matrix<unsigned int, 3, 3>;
using Mat4Ui = GLESC::Math::Matrix<unsigned int, 4, 4>;

using Mat2L = GLESC::Math::Matrix<long, 2, 2>;
using Mat3L = GLESC::Math::Matrix<long, 3, 3>;
using Mat4L = GLESC::Math::Matrix<long, 4, 4>;

using Mat2Ul = GLESC::Math::Matrix<unsigned long, 2, 2>;
using Mat3Ul = GLESC::Math::Matrix<unsigned long, 3, 3>;
using Mat4Ul = GLESC::Math::Matrix<unsigned long, 4, 4>;

using Mat2F = GLESC::Math::Matrix<float, 2, 2>;
using Mat3F = GLESC::Math::Matrix<float, 3, 3>;
using Mat4F = GLESC::Math::Matrix<float, 4, 4>;


namespace GLESC::Math {
    inline Mat4D
    perspective(double fovRadians, double aspectRatio, double nearPlane, double farPlane) {
        Mat4D result;
        
        double f = 1.0 / tan(fovRadians / 2.0);
        
        result[0][0] = f / aspectRatio;
        result[1][1] = f;
        result[2][2] = (farPlane + nearPlane) / (nearPlane - farPlane);
        result[2][3] = (2.0 * farPlane * nearPlane) / (nearPlane - farPlane);
        result[3][2] = -1.0;
        result[3][3] = 0.0;
        
        return result;
    }
}