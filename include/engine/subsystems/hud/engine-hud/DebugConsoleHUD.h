/**************************************************************************************************
 * @file   DebugConsoleHUD.h
 * @author Valentin Dumitru
 * @date   07/03/2024
 * @brief  Add description of this file if needed @TODO
 *
 * Copyright (c) 2024$ Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/
#pragma once
#include <imgui/imgui.h>
#include "engine/subsystems/hud/InGameWindow.h"


class DebugConsoleHUD : public GLESC::InGameWindow {
public:
    DebugConsoleHUD();

private:
    void windowContent() override;
    ImVec4 errorColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 warningColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 infoColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
};
