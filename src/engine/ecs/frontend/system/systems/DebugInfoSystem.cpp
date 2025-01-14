#ifndef NDEBUG_GLESC
#include "engine/ecs/frontend/system/systems/DebugInfoSystem.h"

#include "engine/ecs/frontend/component/TransformComponent.h"
#include "engine/subsystems/ingame-debug/EntityListManager.h"
#include "engine/subsystems/ingame-debug/EntityStatsManager.h"

using namespace GLESC::ECS;

DebugInfoSystem::DebugInfoSystem(ECSCoordinator& ecs, Render::Renderer& renderer) : System(ecs, "DebugInfoSystem"),
    renderer(renderer) {
    addComponentRequirement<TransformComponent>();
}

void DebugInfoSystem::setEntityData(const EntityName& name) {
    EntityStatsManager::EntityData entityData;
    EntityID entityId = getEntity(name);
    if(selectedEntity == entityId) return;

    entityData.name = name;
    std::vector<IComponent*> components = getComponents(entityId);
    for (IComponent* component : components) {
        EntityStatsManager::ComponentData componentData;
        componentData.name = component->getName();
        componentData.values = &component->getDebuggingValues();
        componentData.getUpdatedValues = [component]{return component->getUpdatedDebuggingValues();};
        entityData.components.push_back(componentData);
    }
    selectedEntity = entityId;
    EntityStatsManager::setEntityData(entityData);
}


void DebugInfoSystem::update() {
    for (EntityID id : getAssociatedEntities()) {
        EntityListManager::addEntity(getEntityName(id), getEntityMetadata(id).type);
    }
    if(!EntityListManager::getSelectedEntity().empty())
        setEntityData(EntityListManager::getSelectedEntity());
}

#endif
