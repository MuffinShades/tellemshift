#include "Shader.hpp"
#include <cstring>

Shader::Shader(const char *vertex_data, const char *fragment_data) {
    std::cout << "Creating Shaders: " << vertex_data << " " << fragment_data << std::endl;
	//vertex and fragment shaders
	unsigned int vertex, fragment;

	//create vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);

	//add source code to shader
	glShaderSource(vertex, 1, &vertex_data, NULL);
	glCompileShader(vertex);

	//check for errors
	__error_check(vertex, "VERTEX");

    //fragment

	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	//add source code to fragment shader
	glShaderSource(fragment, 1, &fragment_data, NULL);
	glCompileShader(fragment);
	//check for errors
	__error_check(fragment, "FRAGMENT");

	//create the program
	this->PGRM = glCreateProgram();
	//add the shaders
	glAttachShader(this->PGRM, vertex);
	glAttachShader(this->PGRM, fragment);
	//link the program to the GPU
	glLinkProgram(this->PGRM);
	//check errors
	__error_check(this->PGRM, "PROGRAM");

	//delete shaders since were done
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

i32 Shader::SetVec2(std::string label, vec2 *v) {
    if (this->PGRM == NULL)
        return 1;

    glUniform2f(glGetUniformLocation(this->PGRM, label.c_str()), v->x, v->y);

    return 0;
}

i32 Shader::SetVec3(std::string label, vec3 *v) {
    if (this->PGRM == NULL)
        return 1;

    glUniform3f(glGetUniformLocation(this->PGRM, label.c_str()), v->x, v->y, v->z);

    return 0;
}

i32 Shader::SetVec4(std::string label, vec4 *v) {
    if (this->PGRM == NULL)
        return 1;

    glUniform4f(glGetUniformLocation(this->PGRM, label.c_str()), v->x, v->y, v->z, v->w);

    return 0;
}

i32 Shader::SetMat4(std::string label, mat4 *m) {
    if (this->PGRM == NULL || m->m == nullptr)
        return 1;

    glUniformMatrix4fv(glGetUniformLocation(this->PGRM, label.c_str()), 1, GL_FALSE, m->glPtr());

    return 0;
}

i32 Shader::SetiVec2(std::string label, vec2 *v) {
    if (this->PGRM == NULL)
        return 1;

    glUniform2i(glGetUniformLocation(this->PGRM, label.c_str()), (i32) v->x, (i32) v->y);

    return 0;
}

i32 Shader::SetiVec3(std::string label, vec3 *v) {
    if (this->PGRM == NULL)
        return 1;

    glUniform3i(glGetUniformLocation(this->PGRM, label.c_str()), (i32) v->x, (i32) v->y, (i32) v->z);

    return 0;
}

i32 Shader::SetiVec4(std::string label, vec4 *v) {
    if (this->PGRM == NULL)
        return 1;

    glUniform4i(glGetUniformLocation(this->PGRM, label.c_str()), (i32) v->x, (i32) v->y, (i32) v->z, (i32) v->w);

    return 0;
}

i32 Shader::SetBool(std::string label, bool b) {
    return this->SetInt(label, (i32)b);
}

i32 Shader::SetInt(std::string label, i32 val) {
    if (this->PGRM == NULL)
        return 1;

    glUniform1i(glGetUniformLocation(this->PGRM, label.c_str()), val);

    return 0;
}

i32 Shader::SetFloat(std::string label, f32 v) {
    if (this->PGRM == NULL)
        return 1;

    glUniform1f(glGetUniformLocation(this->PGRM, label.c_str()), v);

    return 0;
}

void Shader::use() {
    glUseProgram(this->PGRM);
}