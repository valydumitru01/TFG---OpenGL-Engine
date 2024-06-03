/*******************************************************************************
 *
 * Copyright (c) 2023.
 * Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 ******************************************************************************/

#pragma once

#include "engine/ecs/backend/component/IComponent.h"
#include "engine/subsystems/physics/Physics.h"
#include "engine/subsystems/physics/Collider.h"
#include "engine/subsystems/physics/PhysicsTypes.h"

namespace GLESC::ECS {
    struct PhysicsComponent : IComponent {
        Physics::Physics physics;
        Physics::Collider collider;

        std::string toString() const override {
            return physics.toString();
        }

        std::string getName() const override {
            return "PhysicsComponent";
        }

        void setDebuggingValues() override {
            for (auto& value : physics.getDebuggingValues()) {
                values.push_back(value);
            }
        }

    };
} // namespace GLESC::ECS
