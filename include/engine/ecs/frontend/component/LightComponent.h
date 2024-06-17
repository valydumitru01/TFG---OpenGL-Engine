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
#include "engine/ecs/backend/component/IComponent.h"
#include "engine/subsystems/renderer/lighting/LightSpot.h"

namespace GLESC::ECS {
    struct LightComponent : IComponent {
        Render::LightSpot light;

        [[nodiscard]] std::string getName() const override {
            return "LightComponent";
        }

        [[nodiscard]] std::string toString() const override {
            return light.toString();
        }
#ifndef NDEBUG_GLESC
        void setDebuggingValues() override {
            for (auto& value : light.getDebuggingValues()) {
                values.push_back(value);
            }
        }
#endif
    }; // struct LightComponent
}
