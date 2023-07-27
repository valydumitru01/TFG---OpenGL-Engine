#include <utility>

#include "engine/ecs/systems/RenderSystem.h"

RenderSystem::RenderSystem(Renderer &renderer) :renderer(renderer), System("Render System") {
    addComponentRequirement <TransformComponent>();
    addComponentRequirement <RenderComponent>();
};


void RenderSystem::update(const double timeOfFrame) {
    renderer.getShaderManager().use();
    
    for (auto &entity: getAssociatedEntities()) {
        auto &render = getComponent <RenderComponent>(entity);
        auto &transform = getComponent <TransformComponent>(entity);
        
        renderer.renderMesh(render.mesh, transform.position, transform.rotation, transform.scale);
        
        renderer.renderTexture(render.textureID, transform.position, transform.rotation, transform.scale);
        
        render.textureID = renderer.getTextureManager().loadTexture(render.texturePath);
        
        renderer.getTextureManager().bindTexture(render.textureID);
        
        // Set the texture uniform
        renderer.getShaderManager().setInt("texture1", static_cast<int>(render.textureID));
        
        // Draw the vertices
        GlApi::drawElements(render.mesh.getIndexCount());
        
        // TODO: Batch rendering
    }
}



void RenderSystem::bindMeshBuffers(Mesh &mesh) {
    GlApi::bindVertexArray(mesh.VAO);
    GlApi::bindVertexBuffer(mesh.VBO);
    GlApi::bindElementBuffer(mesh.EBO);
}
