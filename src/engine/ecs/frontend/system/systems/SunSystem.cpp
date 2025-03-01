#include "engine/ecs/frontend/system/systems/SunSystem.h"

#include "engine/ecs/frontend/component/TransformComponent.h"
#include "engine/ecs/frontend/component/SunComponent.h"
#include "engine/subsystems/ingame-debug/HudItemsManager.h"

namespace GLESC::ECS {
    SunSystem::SunSystem(ECSCoordinator& ecs, Render::Renderer& renderer) :
        System(ecs, "SunSystem"), renderer(renderer) {
        addComponentRequirement<SunComponent>();
        addComponentRequirement<TransformComponent>();
    };

    void SunSystem::update() {
        const std::set<EntityID>& entities = getAssociatedEntities();
        D_ASSERT_TRUE(entities.size() <= 1, "For now, only one sun is supported.");
        if (renderer.hasRenderBeenCalledThisFrame())
            for (auto& entity : entities) {
                auto& sun = getComponent<SunComponent>(entity);
                auto& transform = getComponent<TransformComponent>(entity);
                renderer.setSun(sun.sun, sun.globalAmbientLight, transform.transform);
                HudItemsManager::addItem(HudItemType::SUN, transform.transform.getPosition());
            }
    }
} // namespace GLESC::ECS
