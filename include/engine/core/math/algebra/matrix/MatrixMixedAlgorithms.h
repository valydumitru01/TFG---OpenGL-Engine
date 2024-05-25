/******************************************************************************
 * @file   MatrixMixedAlgorithms.h
 * @author Valentin Dumitru
 * @date   2023-10-31
 * @brief  This is needed to avoid circular dependencies between MatrixMixedAlgorithms and VectorMixedAlgorithms.
 * @see VectorMixedAlgorithms.h for more details on why this is needed.
 *
 * Copyright (c) 2023 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 ******************************************************************************/
#pragma once


#include "engine/core/asserts/Asserts.h"
#include "engine/core/math/Math.h"
#include "engine/core/math/algebra/matrix/MatrixAlgorithms.h"
#include "engine/core/math/algebra/matrix/MatrixTypes.h"
#include "engine/core/math/algebra/vector/VectorAlgorithms.h"

namespace GLESC::Math {
    class MatrixMixedAlgorithms {
    public:
        template <typename Type, size_t N>
        static void
        matrixToVector(const MatrixData<Type, N, 1>& matrix, VectorData<Type, N>& vector) {
            for (size_t i = 0; i < N; ++i) {
                vector[i] = matrix[i][0];
            }
        }


        template <typename Type>
        static void lookAt3D(const MatrixData<Type, 4, 4>& transformMatrix,
                             const VectorData<Type, 3>& targetPos,
                             const VectorData<Type, 3>& upVector,
                             MatrixData<Type, 4, 4>& resMatrix) {
            VectorData<Type, 3> eye = {transformMatrix[0][3], transformMatrix[1][3], transformMatrix[2][3]};
            MatrixMixedAlgorithms::lookAt3D(eye, targetPos, upVector, resMatrix);
        }

        /**
         * @brief Calculate the view matrix from the eye, target, and up vectors.
         * @details This function calculates the view matrix from the eye, target, and up vectors.
         * The function assumes a specific order of transformations, scale, then rotation, then translation.
         *
         * @tparam TypeMat The data type of the matrix elements (e.g., float, double).
         * @tparam TypeEye The data type of the eye vector elements (e.g., float, double).
         * @tparam TypeTarget The data type of the target vector elements (e.g., float, double).
         * @tparam TypeUp The data type of the up vector elements (e.g., float, double).
         * @param eye A 3D vector containing the position of the camera.
         * @param target A 3D vector containing the position of the target.
         * @param up A 3D vector containing the world's up direction. (usually {0, 1, 0})
         * @param resMatrix A 4x4 matrix which will contain the result of the view matrix.
         */
        template <typename TypeMat, typename TypeEye, typename TypeTarget, typename TypeUp>
        static void lookAt3D(const VectorData<TypeEye, 3>& eye,
                             const VectorData<TypeTarget, 3>& target,
                             const VectorData<TypeUp, 3>& up,
                             MatrixData<TypeMat, 4, 4>& resMatrix) {
            VectorData<TypeMat, 3> f;
            VectorAlgorithms::vectorSub(target, eye, f); // forward = target - eye
            VectorAlgorithms::normalize(f, f);

            VectorData<TypeMat, 3> s; // s = right
            VectorAlgorithms::crossProduct(f, up, s);
            VectorAlgorithms::normalize(s, s);

            VectorData<TypeMat, 3> u; // u = up
            VectorAlgorithms::crossProduct(s, f, u);

            resMatrix = {
                {
                    {s[0], u[0], -f[0], 0},
                    {s[1], u[1], -f[1], 0},
                    {s[2], u[2], -f[2], 0},
                    {
                        -VectorAlgorithms::dotProduct(s, eye),
                        -VectorAlgorithms::dotProduct(u, eye),
                        VectorAlgorithms::dotProduct(f, eye),
                        1
                    }
                }
            };
        }

        /**
         * @brief Rotate a 3D matrix around the X, Y, and Z axis.
         * @details This function applies sequential rotations to the input matrix based on the given angles.
         * The function assumes a specific order of rotations. It is Z, then Y, then X
         * TODO: Be cautious about gimbal lock when using Euler angles.
         *   It's recommended to use quaternions for robustness.
         * @tparam TypeDgrs The data type of the rotation angles (e.g., float, double).
         * @tparam TypeRes The data type of the result matrix elements (e.g., float, double).
         * @param rads A 3D vector containing the rotation angles around the X, Y, and Z axes respectively.
         * @param result A 4x4 matrix which will contain the result of the rotation.
         */
        template <typename TypeDgrs, typename TypeRes>
        static void getRotate3DMatrixForAxis(TypeDgrs rads,
                                             const VectorData<TypeDgrs, 3>& axisVec,
                                             MatrixData<TypeRes, 4, 4>& result) {
            const TypeDgrs angleRadians = rads;
            const TypeRes c = Math::cos(angleRadians);
            const TypeRes s = Math::sin(angleRadians);
            const TypeRes t = TypeRes(1) - c;

            // Normalized axis vector
            VectorData<TypeDgrs, 3> axis;
            VectorAlgorithms::normalize(axisVec, axis);

            VectorData<TypeRes, 3> temp;
            VectorAlgorithms::vectorScalarMul(axis, t, temp);

            result[0][0] = c + temp[0] * axis[0];
            result[0][1] = temp[0] * axis[1] + s * axis[2];
            result[0][2] = temp[0] * axis[2] - s * axis[1];
            result[0][3] = 0;

            result[1][0] = temp[1] * axis[0] - s * axis[2];
            result[1][1] = c + temp[1] * axis[1];
            result[1][2] = temp[1] * axis[2] + s * axis[0];
            result[1][3] = 0;

            result[2][0] = temp[2] * axis[0] + s * axis[1];
            result[2][1] = temp[2] * axis[1] - s * axis[0];
            result[2][2] = c + temp[2] * axis[2];
            result[2][3] = 0;

            result[3][0] = 0;
            result[3][1] = 0;
            result[3][2] = 0;
            result[3][3] = 1;
        }

        template <typename TypeDgrs, typename TypeRes>
        static void getRotate3DMatrix(const VectorData<TypeDgrs, 3>& rads,
                                      MatrixData<TypeRes, 4, 4>& result) {
            GLESC::Math::MatrixAlgorithms::setMatrixZero(result);
            GLESC::Math::MatrixAlgorithms::setMatrixDiagonal(result, 1.0f);

            MatrixData<TypeRes, 4, 4> rotX;
            VectorData<TypeDgrs, 3> rotXAxis = {1, 0, 0};
            MatrixMixedAlgorithms::getRotate3DMatrixForAxis(rads[0], rotXAxis, rotX);
            MatrixData<TypeRes, 4, 4> rotY;
            VectorData<TypeDgrs, 3> rotYAxis = {0, 1, 0};
            MatrixMixedAlgorithms::getRotate3DMatrixForAxis(rads[1], rotYAxis, rotY);
            MatrixData<TypeRes, 4, 4> rotZ;
            VectorData<TypeDgrs, 3> rotZAxis = {0, 0, 1};
            MatrixMixedAlgorithms::getRotate3DMatrixForAxis(rads[2], rotZAxis, rotZ);

            MatrixAlgorithms::matrixMatrixMulInPlace(result, rotX, result);
            MatrixAlgorithms::matrixMatrixMulInPlace(result, rotY, result);
            MatrixAlgorithms::matrixMatrixMulInPlace(result, rotZ, result);
        }

        /**
         * @brief Calculate the model matrix from the position, rotation, and scale.
         * @details This function calculates the model matrix from the position, rotation, and scale.
         * The function applies the transformations in the following order: scale, rotation, and translation.
         * The function assumes a specific order of transformations: scale, then rotation, then translation.
         * As we're working with row-major matrices, the order of transformations in matrix multiplications is reversed.
         *
         * @tparam ModelType The data type of the matrix and vector elements (e.g., float, double).
         * @tparam TypePos The data type of the position vector elements (e.g., float, double).
         * @tparam TypeRot The data type of the rotation vector elements (e.g., float, double).
         * @tparam TypeScale The data type of the scale vector elements (e.g., float, double).
         * @param position A 3D vector containing the position of the model.
         * @param rotationRads A 3D vector containing the rotation angles (in radians) around X, Y, and Z axes respectively.
         * @param scale A 3D vector containing the scale factors for the model.
         * @param model A 4x4 matrix which will contain the result of the model matrix.
         */
        template <typename ModelType, typename TypePos, typename TypeRot, typename TypeScale>
        static void calculateModelMatrix(const VectorData<TypePos, 3>& position,
                                         const VectorData<TypeRot, 3>& rotationRads,
                                         const VectorData<TypeScale, 3>& scale,
                                         MatrixData<ModelType, 4, 4>& model) {
            // Initialize matrices
            MatrixData<ModelType, 4, 4> scaleMatrix;
            MatrixData<ModelType, 4, 4> rotationMatrix;
            MatrixData<ModelType, 4, 4> translationMatrix;
            MatrixAlgorithms::setMatrixZero(model);
            MatrixAlgorithms::setMatrixDiagonal(model, ModelType(1));

            // First, create the scale matrix
            MatrixAlgorithms::getScaleMatrix(scale, scaleMatrix);

            // Create rotation matrices around X, Y, and Z axes
            MatrixMixedAlgorithms::getRotate3DMatrix(rotationRads, rotationMatrix);

            // Create the translation matrix
            MatrixAlgorithms::getTranslationMatrix(position, translationMatrix);

            MatrixAlgorithms::matrixMatrixMulInPlace(model, translationMatrix, model);

            MatrixAlgorithms::matrixMatrixMulInPlace(model, rotationMatrix, model);

            MatrixAlgorithms::matrixMatrixMulInPlace(model, scaleMatrix, model);
        }

        template <typename TypePos, typename TypeRot, typename TypeRes>
        static void calculateViewMatrixPosRot(const VectorData<TypePos, 3>& position,
                                              const VectorData<TypeRot, 3>& rotationRads,
                                              MatrixData<TypeRes, 4, 4>& viewMat) {
            // Negate position for translation
            VectorData<TypePos, 3> negatedPos;
            negatedPos[0] = -position[0];
            negatedPos[1] = -position[1];
            negatedPos[2] = -position[2];

            VectorData<TypeRes, 3> negatedRotationRadians;
            negatedRotationRadians[0] = -rotationRads[0];
            negatedRotationRadians[1] = -rotationRads[1];
            negatedRotationRadians[2] = -rotationRads[2];

            VectorData<TypeRes, 3> scale = {1, 1, 1};

            MatrixMixedAlgorithms::calculateModelMatrix(negatedPos, negatedRotationRadians, scale, viewMat);
        }

        template <typename Type>
        static void calculateViewMatrixEye(const VectorData<Type, 3>& eye,
                                           const VectorData<Type, 3>& target,
                                           const VectorData<Type, 3>& up,
                                           MatrixData<Type, 4, 4>& resMatrix) {
            MatrixMixedAlgorithms::lookAt3D(eye, target, up, resMatrix);
        }
    }; // class MatrixAlgorithms
}; // namespace GLESC::Math
