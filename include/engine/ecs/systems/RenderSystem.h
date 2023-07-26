#pragma once

#include "engine/ecs/components/RenderComponent.h"
#include "engine/ecs/components/TransformComponent.h"
#include "System.h"

#include "engine/subsystems/renderer/Renderer.h"


class RenderSystem : public System {
public:
    RenderSystem() : System("Render System") {};
    
    /**
     * @brief Render the content of the screen.
     * Must be called every frame
     * The order of the OpenGL calls is important
     * Here is the order:
     * 1. Clear the buffers with glClear()
     * 2. Bind the shader program with glUseProgram()
     * 3. Set the uniforms with glUniform*() (e.g., glUniformMatrix4fv(), glUniform1i(), etc.)
     * 4. Bind the texture(s) with glBindTexture()
     * 5. Bind the vertex array object with glBindVertexArray()
     * 6. Bind the element array buffer with glBindBuffer()
     * 7. Set the vertex attribute pointers with glVertexAttribPointer() and glEnableVertexAttribArray()
     * 8. Draw the elements with glDrawElements()
     * 9. Unbind res-mng (optional, but recommended for clarity)
     * 10. Swap the buffers to display the rendered frame
     * @param timeOfFrame The time of the frame
     */
    void update(double timeOfFrame);
private:
    
    /**
     * @brief Bind the mesh buffers
     * @details Bind the vertex array object, the vertex buffer object,
     * the element buffer object, and the texture buffer object
     * @param mesh The mesh to bind
     */
    static void bindMeshBuffers(Mesh &mesh);
    /**
     * @brief The renderer
     * @details The renderer is used to access the shader manager, the texture manager, and the windowManager manager
     */
    std::shared_ptr <Renderer> renderer;
};