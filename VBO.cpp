//
// Created by EdwinK on 6/22/2022.
//

#include "VBO.h"

VBO::VBO(const std::vector<GLfloat>& vertices, GLsizeiptr size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    // Store vertices into VBO,
    // total size of data in bytes,
    // GL_STREAM - Vertices modified once and used a few times
    // GL_STATIC - Vertices modified once and used many-many times
    // GL_DYNAMIC - Vertices modified multiple times and used many-many times
    // GL_DRAW - Vertices will be modified and used to draw an image onto the screen
    glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW);

    // Load Data To The Sub-Buffers
    //glBufferSubData(GL_ARRAY_BUFFER, 0)

}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::UnBind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
    glDeleteBuffers(1, &ID);
}

void VBO::UpdateBuffers(const std::vector<GLfloat>& vertices, GLsizeiptr size) {
    // Delete Buffers
    Delete();
    // Create New Buffers
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    // Store vertices into VBO,
    // total size of data in bytes,
    // GL_STREAM - Vertices modified once and used a few times
    // GL_STATIC - Vertices modified once and used many-many times
    // GL_DYNAMIC - Vertices modified multiple times and used many-many times
    // GL_DRAW - Vertices will be modified and used to draw an image onto the screen
    glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW);
}
