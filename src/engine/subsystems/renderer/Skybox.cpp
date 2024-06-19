/**************************************************************************************************
 * @file   Skybox.cpp
 * @author Valentin Dumitru
 * @date   14/06/2024
 * @brief  Add description of this file if needed @TODO
 *
 * Copyright (c) 2023 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/
#include "engine/subsystems/renderer/Skybox.h"
using namespace GLESC::Render;

Skybox::Skybox() : skyboxCubemap(Cubemap(std::array<std::string, 6>{
                       SKYBOX_PATH + std::string("right.png"),
                       SKYBOX_PATH + std::string("left.png"),
                       SKYBOX_PATH + std::string("top.png"),
                       SKYBOX_PATH + std::string("bottom.png"),
                       SKYBOX_PATH + std::string("front.png"),
                       SKYBOX_PATH + std::string("back.png")
                   })), skyboxShader("SkyboxShader.glsl"),
                   skyboxVAOID{0}, skyboxVBOID{0},
                   skyboxVertices{
                       // positions
                       -1.0f, 1.0f, -1.0f,
                       -1.0f, -1.0f, -1.0f,
                       1.0f, -1.0f, -1.0f,
                       1.0f, -1.0f, -1.0f,
                       1.0f, 1.0f, -1.0f,
                       -1.0f, 1.0f, -1.0f,

                       -1.0f, -1.0f, 1.0f,
                       -1.0f, -1.0f, -1.0f,
                       -1.0f, 1.0f, -1.0f,
                       -1.0f, 1.0f, -1.0f,
                       -1.0f, 1.0f, 1.0f,
                       -1.0f, -1.0f, 1.0f,

                       1.0f, -1.0f, -1.0f,
                       1.0f, -1.0f, 1.0f,
                       1.0f, 1.0f, 1.0f,
                       1.0f, 1.0f, 1.0f,
                       1.0f, 1.0f, -1.0f,
                       1.0f, -1.0f, -1.0f,

                       -1.0f, -1.0f, 1.0f,
                       -1.0f, 1.0f, 1.0f,
                       1.0f, 1.0f, 1.0f,
                       1.0f, 1.0f, 1.0f,
                       1.0f, -1.0f, 1.0f,
                       -1.0f, -1.0f, 1.0f,

                       -1.0f, 1.0f, -1.0f,
                       1.0f, 1.0f, -1.0f,
                       1.0f, 1.0f, 1.0f,
                       1.0f, 1.0f, 1.0f,
                       -1.0f, 1.0f, 1.0f,
                       -1.0f, 1.0f, -1.0f,

                       -1.0f, -1.0f, -1.0f,
                       -1.0f, -1.0f, 1.0f,
                       1.0f, -1.0f, -1.0f,
                       1.0f, -1.0f, -1.0f,
                       -1.0f, -1.0f, 1.0f,
                       1.0f, -1.0f, 1.0f
                   } {
    size_t skyboxVerticesCount = sizeof(skyboxVertices) / sizeof(float);
    size_t skyboxVerticesSize = sizeof(float);
    calculateAverageColor();
    skyboxCubemap.release();
    skyboxVAO = std::make_unique<VertexArray>();
    skyboxVAO->bind();
    skyboxVBO = std::make_unique<VertexBuffer>(
        &skyboxVertices,
        skyboxVerticesCount,
        skyboxVerticesSize,
        Enums::BufferUsages::StaticDraw);

    VertexBufferLayout skyboxLayout;
    skyboxLayout.push(Enums::Types::Vec3F);
    skyboxVAO->addBuffer(*skyboxVBO.get(), skyboxLayout);
}


#define PIXEL_SAMPLE_COUNT 20

void Skybox::calculateAverageColor() {
    // To calculate the average color of the skybox we'll get 20 pixels at random
    // and calculate the average color of those pixels
    // (technicall its gonna be 120 as we get 6 pixels per sample)
    for (int i = 0; i < PIXEL_SAMPLE_COUNT; i++) {
        // Its a cube, with the width is enough, no need more checking
        Size pixelWidth = skyboxCubemap.getCubemapTextures()[0].getWidth();
        Size x = Math::generateRandomNumber(static_cast<Size>(0), pixelWidth - 1);
        Size y = Math::generateRandomNumber(static_cast<Size>(0), pixelWidth - 1);
        std::array<Pixel, 6> pixels = skyboxCubemap.getPixelsAtCoords(x, y);
        ColorRgb finalColor;
        for (int pix = 0; pix < 6; pix++) {
            ColorRgb pixColor =
                ColorRgb(pixels[pix].r, pixels[pix].g, pixels[pix].b);
            finalColor += pixColor;
        }
        finalColor /= pixels.size();
        averageColor += finalColor;
    }
    averageColor /= PIXEL_SAMPLE_COUNT;
}

void Skybox::draw(const View& view, const Projection& projection) const {
    Mat3F resizedView(view);
    VP viewTranslationless(resizedView);

    // change depth function so depth test passes when values are equal to depth buffer's content
    getGAPI().setDepthFunction(Enums::DepthFuncs::LessEqual);
    skyboxShader.bind();
    skyboxShader.setUniform("uVPTranslationless", projection * viewTranslationless);
    skyboxVAO->bind();
    skyboxCubemap.bind();
    getGAPI().drawTriangles(0, 36);
    getGAPI().setDepthFunction(Enums::DepthFuncs::Less);
}