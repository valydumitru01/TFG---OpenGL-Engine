/******************************************************************************
 * @file   Example.h
 * @author Valentin Dumitru
 * @date   2023-09-26
 * @brief @todo
 *
 * Copyright (c) 2023 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 ******************************************************************************/

#include "engine/core/low-level-renderer/buffers/vertex/VertexBuffer.h"
using namespace GLESC;


VertexBuffer::VertexBuffer(const std::any *data,
                           GAPIsize size){
    
    gapi.genBuffers(1, vertexBufferID);
    gapi.bindBuffer(GAPIValues::BufferTypeVertex, vertexBufferID);
    gapi.setBufferData(data, size, vertexBufferID, GAPIValues::BufferTypeVertex,
                           GAPIValues::BufferUsageStatic);
    gapi.unbindBuffer(GAPIValues::BufferTypeVertex);
}

VertexBuffer::~VertexBuffer() {
    gapi.deleteBuffer(vertexBufferID);
}

void VertexBuffer::bind() const {
    gapi.bindBuffer(GAPIValues::BufferTypeVertex, vertexBufferID);
}

void VertexBuffer::unbind() const {
    gapi.unbindBuffer(GAPIValues::BufferTypeVertex);
}

void VertexBuffer::setData(const std::any *data, GAPIsize size) {
    gapi.bindBuffer(GAPIValues::BufferTypeVertex, vertexBufferID);
    gapi
            .setBufferData(data, size, vertexBufferID, GAPIValues::BufferTypeVertex,
                           GAPIValues::BufferUsageStatic);
    gapi.unbindBuffer(GAPIValues::BufferTypeVertex);
}