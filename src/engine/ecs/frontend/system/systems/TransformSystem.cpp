#include "engine/ecs/frontend/system/systems/TransformSystem.h"

#include "engine/ecs/frontend/component/TransformComponent.h"

namespace GLESC::ECS {
    TransformSystem::TransformSystem(ECSCoordinator& ecs) : System(ecs, "TransformSystem") {
        addComponentRequirement<TransformComponent>();
    }

    void TransformSystem::update() {
        for (auto& entity : getAssociatedEntities()) {
            auto& transform = getComponent<TransformComponent>(entity);
            transform.transform.setOwnerName(getEntityName(entity).c_str());
            Transform::Rotation rotation = transform.transform.getRotation();
            // Use of fmod to avoid floating point errors
            if (rotation.getX() < -360.0f)
                transform.transform.setRotation(Transform::RotationAxis::Pitch, 360.0f);
            if (rotation.getY() < -360.0f)
                transform.transform.setRotation(Transform::RotationAxis::Yaw, 360.0f);
            if (rotation.getZ() < -360.0f)
                transform.transform.setRotation(Transform::RotationAxis::Roll, 360.0f);

            if (rotation.getX() > 360.0f)
                transform.transform.setRotation(Transform::RotationAxis::Pitch, -360.0f);
            if (rotation.getY() > 360.0f)
                transform.transform.setRotation(Transform::RotationAxis::Yaw, -360.0f);
            if (rotation.getZ() > 360.0f)
                transform.transform.setRotation(Transform::RotationAxis::Roll, -360.0f);

        }
    }
} // namespace GLESC::ECS
