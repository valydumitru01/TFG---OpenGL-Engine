/******************************************************************************
 * @file   RenderSystem.h
 * @author Valentin Dumitru
 * @date   2023-09-26
 * @brief @todo
 *
 * Copyright (c) 2023 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 ******************************************************************************/

#pragma once

#include "engine/ecs/frontend/system/System.h"
#include "engine/subsystems/renderer/Renderer.h"


namespace GLESC::ECS {
    class RenderSystem : public System {
    public:
        explicit RenderSystem(Renderer& renderer, ECSCoordinator& ecs);
        void update() override;

    private:
        Renderer& renderer;
    };
}
