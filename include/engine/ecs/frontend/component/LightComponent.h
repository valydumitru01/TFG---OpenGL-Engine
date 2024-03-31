/**************************************************************************************************
 * @file   LightComponent.h
 * @author Valentin Dumitru
 * @date   2024-03-31
 * @brief  Add description of this file if needed @TODO 
 *
 * Copyright (c) 2024 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/
#pragma once
#include "engine/core/debugger/Stringer.h"
#include "engine/ecs/frontend/component/IComponent.h"
#include "engine/subsystems/renderer/lighting/Light.h"

namespace GLESC::ECS {
    struct LightComponent : public IComponent {
        Light light;

        [[nodiscard]] std::string getName() const override {
            return "LightComponent";
        }

        [[nodiscard]] std::string toString() const override {
            return light.toString();
        }
    }; // struct LightComponent
}
