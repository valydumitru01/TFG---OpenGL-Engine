/**************************************************************************************************
 * @file   SceneManager.h
 * @author Valentin Dumitru
 * @date   13/05/2024
 * @brief  Add description of this file if needed @TODO
 *
 * Copyright (c) 2023 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/
#pragma once
#include "SceneTypes.h"
#include "engine/ecs/frontend/entity/EntityFactory.h"
#include "engine/core/window/WindowManager.h"

namespace GLESC::Scene {
    class Scene;

    class SceneManager {
    public:
        SceneManager(ECS::EntityFactory& entityFactory,
                     WindowManager& windowManager)
            : entityFactory(entityFactory),
              windowManager(windowManager) {
        }

        void addScene(const std::string& sceneName, SceneID sceneID) {
            scenes[sceneName] = sceneID;
        }

        void switchScene(const std::string& sceneName) {
            auto it = scenes.find(sceneName);
            D_ASSERT_TRUE(it != scenes.end(), "Scene not found");
            currentScene = it->second;
            windowManager.setWindowTitle(sceneName);
        }

        SceneID& getCurrentScene() {
            return currentScene;
        }

    private:
        ECS::EntityFactory& entityFactory;
        WindowManager& windowManager;
        SceneID currentScene{-1};
        std::unordered_map<std::string, SceneID> scenes{};
    }; // class SceneManager
}; // namespace GLESC::Scene
