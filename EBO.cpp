//
// Created by EdwinK on 6/22/2022.
//

#include "EBO.h"

EBO::EBO(const std::vector<GLuint>& indices, GLsizeiptr size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, &indices[0], GL_STATIC_DRAW);

    // Load Data to Sub-Buffers
    //glBufferSubData(GL_ARRAY_BUFFER, 0, size, &indices[0]);
}

void EBO::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::UnBind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
    glDeleteBuffers(1, &ID);
}
