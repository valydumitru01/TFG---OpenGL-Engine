/**************************************************************************************************
 * @file   DebugItemsHUD.h
 * @author Valentin Dumitru
 * @date   2024-04-17
 * @brief  Add description of this file if needed @TODO 
 *
 * Copyright (c) 2024 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/
#pragma once
#include <unordered_map>

#include "../../renderer/texture/Texture.h"
#include "engine/subsystems/hud/InGameWindow.h"
#include "engine/subsystems/ingame-debug/HudItemsManager.h"
#include "engine/subsystems/renderer/Renderer.h"
#include "engine/subsystems/renderer/texture/TextureFactory.h"

enum class HudItemType;

namespace GLESC::HUD {
    class DebugItems : public InGameWindow {
    public:
        DebugItems(Render::Renderer& renderer, TextureFactory& textureFactory);

    private:
        void windowContent() override;
        TextureFactory& textureFactory;
        Render::Renderer& renderer;
        std::unordered_map<HudItemType, GAPITexture*> items;
    }; // class DebugItemsHUD
} // namespace GLESC::HUD
