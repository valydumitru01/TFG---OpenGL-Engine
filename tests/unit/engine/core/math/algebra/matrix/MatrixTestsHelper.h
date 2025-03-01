/**************************************************************************************************
 * @file   MatrixTestsHelper.cpp
 * @author Valentin Dumitru
 * @date   16/01/2024
 * @brief  @TODO Add description of this file if needed
 *
 * Copyright (c) 2024 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/


#pragma once
#include <gtest/gtest.h>
#include "engine/core/math/algebra/matrix/Matrix.h"


template <typename T, size_t N, size_t M>
struct MatrixType {
    using ValueType = T;
    static const size_t Rows = N;
    static const size_t Cols = M;
};


using MyTypes = ::testing::Types<
    MatrixType<float, 2, 2>,
    MatrixType<double, 2, 2>,
    MatrixType<long double, 2, 2>,
    MatrixType<float, 3, 3>,
    MatrixType<double, 3, 3>,
    MatrixType<long double, 3, 3>,
    MatrixType<float, 4, 4>,
    MatrixType<double, 4, 4>,
    MatrixType<long double, 4, 4>,
    // Non-square matrices
    MatrixType<float, 2, 3>,
    MatrixType<double, 2, 3>,
    MatrixType<float, 3, 2>,
    MatrixType<double, 3, 2>,
    MatrixType<float, 4, 3>,
    MatrixType<double, 4, 3>
>;


/**
 * @brief Generate next value for the matrix
 * @details It will generate numbers for the matrices for the tests. It ensures that the matrix
 * is either upper or lower triangular with non-zero diagonal elements, thus guaranteeing a non-zero determinant.
 * @tparam MyType
 * @param i
 * @param j
 * @return The next value for the matrix given the row and column
 */
template <class MyType>
MyType generateNextValue(size_t i, size_t j) {
    if (i == j)
        return static_cast<MyType>(i + 1);
    if (i < j)
        return static_cast<MyType>(j + 1);
    return static_cast<MyType>(i + 1);
}

template <class MyType, size_t N, size_t M>
void initializeMatrixWithValues(GLESC::Math::MatrixData<MyType, N, M>& matrix) {
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            matrix[i][j] = generateNextValue<MyType>(i, j);
}

template <class MyType, size_t N, size_t M>
void initializeMatrixWithValues(GLESC::Math::Matrix<MyType, N, M>& matrix) {
    initializeMatrixWithValues(matrix.data);
}


template <class MyType, size_t N, size_t M>
void initializeMatrixWithDifferentValues(GLESC::Math::MatrixData<MyType, N, M>& matrix) {
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            matrix[i][j] = generateNextValue<MyType>(i + 10, j + 10);
}

template <class MyType, size_t N, size_t M>
void initializeMatrixWithDifferentValues(GLESC::Math::Matrix<MyType, N, M>& matrix) {
    initializeMatrixWithDifferentValues(matrix.data);
}


#define PREPARE_TEST()\
    using Type = typename TypeParam::ValueType; \
    constexpr size_t N = TypeParam::Rows; \
    constexpr size_t M = TypeParam::Cols; \
    using Mat = GLESC::Math::Matrix<Type, N, M>;
