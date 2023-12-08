/******************************************************************************
 * @file   VertexArray.cpp
 * @author Valentin Dumitru
 * @date   2023-11-07
 * @brief  Add description of this file if needed @todo 
 *
 * Copyright (c) 2023 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 ******************************************************************************/
#include "engine/core/low-level-renderer/buffers/vertex/VertexArray.h"

using namespace GLESC;

VertexArray::VertexArray(){
    getGAPI().genVertexArray(vertexArrayID);
}

VertexArray::~VertexArray() {
    destroyOnce();
}

void VertexArray::destroy() {
    destroyOnce();
}

void VertexArray::bind() const {
    getGAPI().bindVertexArray(vertexArrayID);
}

void VertexArray::unbind() const {
    getGAPI().unbindVertexArray();
}


void VertexArray::addBuffer(const GLESC::VertexBuffer &vb,
                            const GLESC::VertexBufferLayout &layout) {
    // Retrieve the vertex attribute elements
    auto const &elements = layout.getElements();
    // Get the stride (byte offset between consecutive attributes)
    auto const &stride = layout.getStride();
    
    GAPI::UInt offset = 0; // Start with an offset of 0
    for (size_t i = 0; i < elements.size(); ++i) {
        // For each element in the layout
        auto const &element = elements[i];
        
        // Get the data from the enum type
        auto const typeCount = static_cast<GAPI::UInt>(getTypeCount(element.type));
        auto const type = getTypePrimitiveType(element.type);
        // The type size can be vectors or matrices, so we get the size from the primitive type
        // and not from the type of the element of the layout because it can be a vector or a matrix
        // and gapi recognizes only the size of the primitive types
        auto const typeSize = static_cast<GAPI::UInt>(getTypeSize(type));
        
        // Set up the vertex attribute pointers
        getGAPI().createVertexData(static_cast<GAPI::UInt>(i),
                              typeCount,
                              type,
                              element.normalized,
                              stride,
                              offset);
        
        // Enable the vertex attribute array
        getGAPI().enableVertexData(static_cast<GAPI::UInt>(i));
        
        // Calculate the offset for the next attribute
        offset += typeCount * typeSize;
    }
}

void VertexArray::destroyOnce() {
    if (objectAlive) {
        getGAPI().deleteVertexArray(vertexArrayID);
        objectAlive = false;
    }
}