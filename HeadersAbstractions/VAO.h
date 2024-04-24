#ifndef AI_MAZE_GRAPHICS_VAO_H
#define AI_MAZE_GRAPHICS_VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
    GLuint ID;
    VAO();
    void LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum Type, GLsizeiptr stride, void *offset);
    void Bind();
    void UnBind();
    void Delete();

};


#endif //AI_MAZE_GRAPHICS_VAO_H
