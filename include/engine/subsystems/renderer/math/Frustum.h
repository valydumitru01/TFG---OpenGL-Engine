/**************************************************************************************************
 * @file   Frustum.h
 * @author Valentin Dumitru
 * @date   11/12/2023
 * @brief  Class for representing a frustum and performing intersection tests.
 *
 * This class represents a viewing frustum, which can be used for frustum culling
 * or visibility determination in 3D scenes. The frustum is defined by six planes.
 **************************************************************************************************/
#pragma once

#include "engine/core/math/geometry/figures/polyhedron/Polyhedron.h"
#include "../../../core/math/geometry/figures/BoundingVolume.h"

namespace GLESC::Render {
    class Frustum {
    public:
        Frustum() = delete;

        [[nodiscard]] const std::array<Math::Plane, 6>& getPlanes() const {
            return planes;
        }

        /**
         * @brief Constructs a frustum from a camera transform and perspective.
         *
         * @param VPMatrix The combined view-projection matrix.
         */
        Frustum(const VP& VPMatrix) {
            extractPlanes(VPMatrix);
        }

        /**
         * @brief Updates the frustum planes based on new view and projection matrices.
         *
         * @param VPMat The combined view-projection matrix.
         */
        void update(const VP& VPMat) {
            extractPlanes(VPMat);
        }

        /**
         * @brief Checks if the frustum intersects with a given bounding volume.
         * @details Returns true if any point is inside all the planes
         *
         * @param volume The bounding volume to test for intersection.
         * @return true If the frustum intersects the volume.
         * @return false If the frustum does not intersect the volume.
         */
        [[nodiscard]] bool contains(const Math::BoundingVolume& volume) const {
            const auto& bbox = volume.getBoundingBox();
            for (const auto& plane : planes) {
                // Check if all points of the AABB are outside a plane
                if (!plane.hasInside({bbox.min.getX(), bbox.min.getY(), bbox.min.getZ()}) &&
                    !plane.hasInside({bbox.max.getX(), bbox.min.getY(), bbox.min.getZ()}) &&
                    !plane.hasInside({bbox.min.getX(), bbox.max.getY(), bbox.min.getZ()}) &&
                    !plane.hasInside({bbox.max.getX(), bbox.max.getY(), bbox.min.getZ()}) &&
                    !plane.hasInside({bbox.min.getX(), bbox.min.getY(), bbox.max.getZ()}) &&
                    !plane.hasInside({bbox.max.getX(), bbox.min.getY(), bbox.max.getZ()}) &&
                    !plane.hasInside({bbox.min.getX(), bbox.max.getY(), bbox.max.getZ()}) &&
                    !plane.hasInside({bbox.max.getX(), bbox.max.getY(), bbox.max.getZ()})) {
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]] bool contains(Position position) const {
            for (const Math::Plane& plane : planes) {
                if (!plane.hasInside(position)) {
                    return false;
                }
            }
            return true;
        }

    private:
        /**
         * @brief Extracts the frustum planes from a combined view-projection matrix. Uses Hartmann & Gribbs method.
         * It's done with row-major matrices.
         * @see https://web.archive.org/web/20210226045225/http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf
         *
         * @param VPMatrix The combined view-projection matrix.
         */
        void extractPlanes(const VP& VPMatrix) {
            if constexpr (Math::MatrixAlgorithms::colMajorMatrix) {
                // Left clipping plane
                planes[0].setNormal(Math::Direction(VPMatrix[3][0] + VPMatrix[0][0],
                                                    VPMatrix[3][1] + VPMatrix[0][1],
                                                    VPMatrix[3][2] + VPMatrix[0][2]));
                planes[0].setDistance(VPMatrix[3][3] + VPMatrix[0][3]);
                planes[0].normalize();

                // Right clipping plane
                planes[1].setNormal(Math::Direction(VPMatrix[3][0] - VPMatrix[0][0],
                                                    VPMatrix[3][1] - VPMatrix[0][1],
                                                    VPMatrix[3][2] - VPMatrix[0][2]));
                planes[1].setDistance(VPMatrix[3][3] - VPMatrix[0][3]);
                planes[1].normalize();

                // Top clipping plane
                planes[2].setNormal(Math::Direction(VPMatrix[3][0] - VPMatrix[1][0],
                                                    VPMatrix[3][1] - VPMatrix[1][1],
                                                    VPMatrix[3][2] - VPMatrix[1][2]));
                planes[2].setDistance(VPMatrix[3][3] - VPMatrix[1][3]);
                planes[2].normalize();

                // Bottom clipping plane
                planes[3].setNormal(Math::Direction(VPMatrix[3][0] + VPMatrix[1][0],
                                                    VPMatrix[3][1] + VPMatrix[1][1],
                                                    VPMatrix[3][2] + VPMatrix[1][2]));
                planes[3].setDistance(VPMatrix[3][3] + VPMatrix[1][3]);
                planes[3].normalize();

                // Near clipping plane
                planes[4].setNormal(Math::Direction(VPMatrix[3][0] + VPMatrix[2][0],
                                                    VPMatrix[3][1] + VPMatrix[2][1],
                                                    VPMatrix[3][2] + VPMatrix[2][2]));
                planes[4].setDistance(VPMatrix[3][3] + VPMatrix[2][3]);
                planes[4].normalize();

                // Far clipping plane
                planes[5].setNormal(Math::Direction(VPMatrix[3][0] - VPMatrix[2][0],
                                                    VPMatrix[3][1] - VPMatrix[2][1],
                                                    VPMatrix[3][2] - VPMatrix[2][2]));
                planes[5].setDistance(VPMatrix[3][3] - VPMatrix[2][3]);
                planes[5].normalize();
            }
            else {
                // Left clipping plane
                planes[0].setNormal(Math::Direction(VPMatrix[0][3] + VPMatrix[0][0],
                                                    VPMatrix[1][3] + VPMatrix[1][0],
                                                    VPMatrix[2][3] + VPMatrix[2][0]));
                planes[0].setDistance(VPMatrix[3][3] + VPMatrix[3][0]);
                planes[0].normalize();

                // Right clipping plane
                planes[1].setNormal(Math::Direction(VPMatrix[0][3] - VPMatrix[0][0],
                                                    VPMatrix[1][3] - VPMatrix[1][0],
                                                    VPMatrix[2][3] - VPMatrix[2][0]));
                planes[1].setDistance(VPMatrix[3][3] - VPMatrix[3][0]);
                planes[1].normalize();

                // Top clipping plane
                planes[2].setNormal(Math::Direction(VPMatrix[0][3] - VPMatrix[0][1],
                                                    VPMatrix[1][3] - VPMatrix[1][1],
                                                    VPMatrix[2][3] - VPMatrix[2][1]));
                planes[2].setDistance(VPMatrix[3][3] - VPMatrix[3][1]);
                planes[2].normalize();

                // Bottom clipping plane
                planes[3].setNormal(Math::Direction(VPMatrix[0][3] + VPMatrix[0][1],
                                                    VPMatrix[1][3] + VPMatrix[1][1],
                                                    VPMatrix[2][3] + VPMatrix[2][1]));
                planes[3].setDistance(VPMatrix[3][3] + VPMatrix[3][1]);
                planes[3].normalize();

                // Near clipping plane
                planes[4].setNormal(Math::Direction(VPMatrix[0][3] + VPMatrix[0][2],
                                                    VPMatrix[1][3] + VPMatrix[1][2],
                                                    VPMatrix[2][3] + VPMatrix[2][2]));
                planes[4].setDistance(VPMatrix[3][3] + VPMatrix[3][2]);
                planes[4].normalize();

                // Far clipping plane
                planes[5].setNormal(Math::Direction(VPMatrix[0][3] - VPMatrix[0][2],
                                                    VPMatrix[1][3] - VPMatrix[1][2],
                                                    VPMatrix[2][3] - VPMatrix[2][2]));
                planes[5].setDistance(VPMatrix[3][3] - VPMatrix[3][2]);
                planes[5].normalize();
            }
        }

        std::array<Math::Plane, 6> planes{};
    }; // class Frustum
} // namespace GLESC::Render
