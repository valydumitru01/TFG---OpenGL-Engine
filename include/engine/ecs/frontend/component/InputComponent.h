/*******************************************************************************
 *
 * Copyright (c) 2023.
 * Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 ******************************************************************************/

#pragma once

#include <unordered_map>

#include "IComponent.h"
#include "engine/subsystems/input/Command.h"
#include "engine/subsystems/input/InputTypes.h"
#include "engine/subsystems/input/InputKeys.h"
#include "engine/subsystems/input/debugger/InputDebugger.h"


struct InputComponent : public IComponent {
    /**
     * @brief Vector of pairs, subscribed keys and bool,true if they are pressed
     * @details This vector is used to store the keys that are subscribed to the input system,
     * this it saves memory as each only stores the keys that are subscribed to it.
     */
    std::unordered_map<GLESC::Key, Command> subscribedKeys;
    /**
     * @brief The mouse position is always stored and updated, no need for subscription
     * @details This is the mouse position, it is always updated and stored, no need for subscription.
     * This is because the position is not a lot of data and it is almost always used.
     */
    MousePosition mousePosition;

    std::string toString() override {
        std::ostringstream ss;

        ss << "Subscribed Keys: ";
        for (const auto& pair : subscribedKeys) {
            ss << "[" << keyToString(pair.first)
               << "] ";

        }

        ss << "Mouse Position: (" << mousePosition.x << ", " << mousePosition.y << ")";

        return ss.str();
    }
};
