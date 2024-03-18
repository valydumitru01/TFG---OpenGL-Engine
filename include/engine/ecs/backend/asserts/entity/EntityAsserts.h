/*******************************************************************************
 *
 * Copyright (c) 2023.
 * Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 ******************************************************************************/

#pragma once

#include "engine/core/asserts/Asserts.h"


/**
 * @brief Asserts that an entity can be created.
 * @detailed This assert is used when we want to check if there are any entities to be created.
 * @param entityName The entity name to be checked.
 */
#define ASSERT_ENTITY_CAN_BE_CREATED(entityName) \
    D_ASSERT_TRUE(canEntityBeCreated(entityName), "No entities to be created")

/**
 * @brief Asserts the entity is alive.
 * @param entityID The entity ID to be checked.
 */
#define ASSERT_ENTITY_IS_ALIVE(entityID) \
    D_ASSERT_TRUE(isEntityAlive(entityID), "Entity is not alive, but should be")

#define ASSERT_ENTITY_EXISTS(entityID) \
    D_ASSERT_TRUE(doesEntityExist(entityID), "Entity doesn't exist but should")

#define ASSERT_ENTITY_DOESNT_EXIST(entityID) \
    D_ASSERT_FALSE(doesEntityExist(entityID), "Entity exists, but it shouldn't")

/**
 * @brief Asserts that there are living entities.
 * @detailed This assert is used when we want to check if there are any living entities left.
 */
#define ASSERT_THERE_ARE_LIVING_ENTITIES() \
    D_ASSERT_TRUE(areThereLivingEntities(), "No living entities")
/**
 * @brief Asserts the entity is not alive.
 * @param entityID The entity ID to be checked.
 */
#define ASSERT_ENTITY_IS_NOT_ALIVE(entityID) \
    D_ASSERT_FALSE(isEntityAlive(entityID), "Entity is alive, but should not be")
/**
 * @brief Asserts that the entity is alive by name.
 * @detailed This assert is used when we want to check if an entity name is stored in the entityIDs map,
 * if it is then the entity is alive.
 * @param entityName The entity name to be checked.
 * @param entityIDs The entityIDs map.
 */
#define ASSERT_ENTITY_NAME_IS_ALIVE(entityName) \
    D_ASSERT_TRUE(isEntityAlive(entityName), "Entity is not alive, but should be")

#define ASSERT_ENTITY_NAME_IS_NOT_ALIVE(entityName) \
    D_ASSERT_FALSE(isEntityAlive(entityName), "Entity is alive, but should not be")
/**
 * @brief Asserts that the component is in range.
 * @detailed This assert is used when we want to check if a component is in the range of the bits in the signature.
 * @param componentID The component ID to be checked.
 * @param signatures The signatures of the entities.
 */
#define ASSERT_COMPONENT_IS_IN_RANGE(componentID) \
    D_ASSERT_TRUE(isComponentInRange(componentID), "Component is out of range")

#define ASSERT_ENTITY_HAS_COMPONENT(entityID, componentID) \
    D_ASSERT_TRUE(doesEntityHaveComponent(entityID, componentID), "Entity does not have component, but should have")

#define ASSERT_ENTITY_DOESNT_HAVE_COMPONENT(entityID, componentID) \
    D_ASSERT_FALSE(doesEntityHaveComponent(entityID, componentID), "Entity has component, but should not have")