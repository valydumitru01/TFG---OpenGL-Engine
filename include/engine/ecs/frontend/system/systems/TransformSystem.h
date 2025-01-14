/**************************************************************************************************
 * @file   TransformSystem.h
 * @author Valentin Dumitru
 * @date   2024-03-21
 * @brief  System that updates the transform of the entities
 *
 * Copyright (c) 2024 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/
#pragma once

#include "engine/ecs/frontend/system/System.h"


namespace GLESC::ECS {
    class TransformSystem : public System {
    public:
        explicit TransformSystem(ECSCoordinator& ecs);
        void update() override;
    };
} // namespace GLESC::ECS