//
// Created by EdwinK on 6/22/2022.
//


#include "VAO.h"

VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum Type, GLsizeiptr stride, void *offset) {
    vbo.Bind();
    // Configure VAO
    // Way of Communicating with the Vertex Shader from the outside
    glVertexAttribPointer(layout, numComponents, Type, GL_FALSE, stride , offset);
    // Enable The Vertex Attribute Pointer
    // 0 - position of the Vertex Attribute
    glEnableVertexAttribArray(layout);
    vbo.UnBind();
}

void VAO::Bind() {
// Bind or Current
    glBindVertexArray(ID);
}

void VAO::UnBind() {
    glBindVertexArray(0);
}

void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}