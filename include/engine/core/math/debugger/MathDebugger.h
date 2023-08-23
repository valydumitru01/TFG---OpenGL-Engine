/*******************************************************************************
 *
 * Copyright (c) 2023 Valentin Dumitru.
 * Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 ******************************************************************************/

#pragma once

#include <sstream>
#include <glm/glm.hpp>
#include "engine/core/debugger/Debugger.h"

namespace GLESC {
    template<>
    std::string toString<glm::mat4>(const glm::mat4 &mat) {
        std::ostringstream out;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                out << mat[i][j] << (j < 3 ? ", " : "");
            }
            out << (i < 3 ? "\n" : "");
        }
        return out.str();
    }
    
    template<>
    std::string toString<glm::mat3>(const glm::mat3 &mat) {
        std::ostringstream out;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                out << mat[i][j] << (j < 2 ? ", " : "");
            }
            out << (i < 2 ? "\n" : "");
        }
        return out.str();
    }
    
    template<>
    std::string toString<glm::vec4>(const glm::vec4 &vec) {
        std::ostringstream out;
        out << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
        return out.str();
    }
    
    template<>
    std::string toString<glm::vec3>(const glm::vec3 &vec) {
        std::ostringstream out;
        out << vec.x << ", " << vec.y << ", " << vec.z;
        return out.str();
    }
    
    template<>
    std::string toString<glm::vec2>(const glm::vec2 &vec) {
        std::ostringstream out;
        out << vec.x << ", " << vec.y;
        return out.str();
    }
}