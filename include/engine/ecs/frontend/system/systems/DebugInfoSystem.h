/**************************************************************************************************
 * @file   DebugInfoSystem.h
 * @author Valentin Dumitru
 * @date   2024-03-24
 * @brief  Add description of this file if needed @TODO 
 *
 * Copyright (c) 2024 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/
#pragma once
#include "engine/ecs/frontend/system/System.h"
#include "engine/subsystems/renderer/Renderer.h"

namespace GLESC::ECS {
    class DebugInfoSystem : public System {
    public:
        /**
         * @brief Default constructor.
         */
        DebugInfoSystem(ECSCoordinator& ecs, Render::Renderer& renderer);


        void update() override;

    private:
        void setEntityData(const EntityName& name);
        EntityID selectedEntity{nullEntity};
        Render::Renderer& renderer;

        float maxDetectionValue = 20.0f;
    }; // class DebugInfoSystem
}
