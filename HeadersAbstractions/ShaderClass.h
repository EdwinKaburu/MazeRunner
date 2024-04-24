#ifndef AI_MAZE_GRAPHICS_SHADERCLASS_H
#define AI_MAZE_GRAPHICS_SHADERCLASS_H

#include <iostream>
#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>


// Function To Read Shader Text Files
// Output contents of a text-file as a string
std::string get_file_contents(const char *filename);

class ShaderClass {
public:
    // public Program ID Reference
    GLuint ID;
    ShaderClass(const char *vertex_file, const char *fragment_file);
    void Activate();
    void Delete();
private:
    void CompileErrors(unsigned int shader, const char *type);
};


#endif //AI_MAZE_GRAPHICS_SHADERCLASS_H