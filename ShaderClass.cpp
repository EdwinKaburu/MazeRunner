//
// Created by EdwinK on 6/22/2022.
//

#include "ShaderClass.h"

std::string get_file_contents(const char *filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

ShaderClass::ShaderClass(const char *vertex_file, const char *fragment_file) {
    // Get Strings from TextFiles
    std::string vertex_code = get_file_contents(vertex_file);
    std::string fragment_code = get_file_contents(fragment_file);

    // Convert to Character Array
    const char* Vertex_Shader_Source = vertex_code.c_str();
    const char* Fragment_Shader_Source = fragment_code.c_str();

    // Reference To Vertex Shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // feed shader-source code
    // Using 1 screen for the shader
    glShaderSource(vertex_shader, 1, &Vertex_Shader_Source, NULL);
    // Compile the Shader Source Code to Machine Code
    glCompileShader(vertex_shader);

    // Reference To Fragment/Pixel Shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    // Link To Shader-Source Code, using 1 screen for the shader
    glShaderSource(fragment_shader, 1, &Fragment_Shader_Source, NULL);
    // Compile the Shader Source-Code to Machine-Code
    glCompileShader(fragment_shader);

    // Wrap the Shaders into a Shader Program
    // Create Shader Program Reference Value
    ID = glCreateProgram();

    // Attach A Shader to A Shader Program
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);

    // Wrap Up the Shader Program
    glLinkProgram(ID);

    // Delete Shader Reference Values that were Created, already in the program
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void ShaderClass::Activate() {
    // Activate the Shader Program
    glUseProgram(ID);
}

void ShaderClass::Delete() {
    glDeleteProgram(ID);
}

void ShaderClass::CompileErrors(unsigned int shader, const char *type) {
// Stores status of compilation
    GLint hasCompiled;
    // Character array to store error message in
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
        }
    }
}