#ifndef AI_MAZE_GRAPHICS_EBO_H
#define AI_MAZE_GRAPHICS_EBO_H

#include <glad/glad.h>
#include <vector>

class EBO {
public:
    GLuint ID;
    EBO(const std::vector<GLuint>& indices, GLsizeiptr size);
    void Bind();
    void UnBind();
    void Delete();
};


#endif //  AI_MAZE_GRAPHICS_EBO_H