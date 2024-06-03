/**
 * @file   EntityFactory.h
 * @author valyd
 * @date   2024-03-29
 * @brief  Brief description of EntityFactory.
 *
 * Copyright (c) 2024 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 */

#pragma once
#include <optional>

#include "Entity.h"

namespace GLESC::ECS {
    class EntityFactory {
    public:
        explicit EntityFactory(ECSCoordinator &ecs) : ecs(ecs) {}

        Entity createEntity(const EntityName &name, const EntityMetadata& metadata);

        Entity createEntity();

        std::optional<Entity> tryGetEntity(const EntityName &name);

        Entity getEntity(const EntityName &name);
        Entity getEntity(const EntityID &id);

    private:
        ECSCoordinator &ecs;
    }; // class EntityFactory
}
