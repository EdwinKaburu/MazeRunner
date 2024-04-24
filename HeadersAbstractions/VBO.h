#ifndef AI_MAZE_GRAPHICS_VBO_H
#define AI_MAZE_GRAPHICS_VBO_H

#include <glad/glad.h>
#include <vector>

class VBO {
public:
    GLuint ID;
    VBO(const std::vector<GLfloat>& vertices, GLsizeiptr size);
    void Bind();
    void UnBind();
    void Delete();
    void UpdateBuffers(const std::vector<GLfloat>& vertices, GLsizeiptr size);

};


#endif //AI_MAZE_GRAPHICS_VBO_H
