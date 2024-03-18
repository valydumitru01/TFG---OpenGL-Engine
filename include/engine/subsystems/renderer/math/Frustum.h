/**************************************************************************************************
 * @file   Frustum.h
 * @author Valentin Dumitru
 * @date   11/12/2023
 * @brief  Add description of this file if needed @TODO
 *
 * Copyright (c) 2023 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/
#pragma once

#include <engine/core/math/geometry/figures/Figure.h>
#include "engine/core/math/geometry/figures/polyhedron/Polyhedron.h"
#include "engine/core/math/algebra/matrix/Matrix.h"
#include "engine/core/math/geometry/figures/plane/Plane.h"

class Frustum {
public:
    /**
     * @brief Construct a new Frustum object from a view projection matrix
     * @details The view projection matrix is used to extract the planes of the frustum.
     * @param vpm
     */
    explicit Frustum(const Mat4D &vpm) {
        extractPlanesFromViewProjectionMatrix(vpm);
    }
    
    
    [[nodiscard]] bool intersects(const BoundingVolume &volume) const {
        // Check each plane for intersection with the volume
        for (const auto &plane : planes) {
            if (!volume.getTopology().intersects(plane)) {
                return false;
            }
        }
        return true;
    }
private:
    void extractPlanesFromViewProjectionMatrix(const Mat4D &vpm) {
        D_ASSERT_TRUE(vpm.isValidViewMatrix(), "Matrix is not a valid view matrix");
        // Left Plane
        planes[0] = GLESC::Math::Plane(Vec3D(vpm[0][3] + vpm[0][0],
                                             vpm[1][3] + vpm[1][0],
                                             vpm[2][3] + vpm[2][0]), vpm[3][3] + vpm[3][0]);
        
        // Right Plane
        planes[1] = GLESC::Math::Plane(Vec3D(vpm[0][3] - vpm[0][0],
                                    vpm[1][3] - vpm[1][0],
                                    vpm[2][3] - vpm[2][0]), vpm[3][3] - vpm[3][0]);
        
        // Bottom Plane
        planes[2] = GLESC::Math::Plane(Vec3D(vpm[0][3] + vpm[0][1],
                                    vpm[1][3] + vpm[1][1],
                                    vpm[2][3] + vpm[2][1]), vpm[3][3] + vpm[3][1]);
        
        // Top Plane
        planes[3] = GLESC::Math::Plane(Vec3D(vpm[0][3] - vpm[0][1],
                                    vpm[1][3] - vpm[1][1],
                                    vpm[2][3] - vpm[2][1]), vpm[3][3] - vpm[3][1]);
        
        // Near Plane
        planes[4] = GLESC::Math::Plane(Vec3D(vpm[0][3] + vpm[0][2],
                                    vpm[1][3] + vpm[1][2],
                                    vpm[2][3] + vpm[2][2]), vpm[3][3] + vpm[3][2]);
        
        // Far Plane
        planes[5] = GLESC::Math::Plane(Vec3D(vpm[0][3] - vpm[0][2],
                                    vpm[1][3] - vpm[1][2],
                                    vpm[2][3] - vpm[2][2]), vpm[3][3] - vpm[3][2]);
    }
    GLESC::Math::Plane planes[6];
    
}; // class Frustum