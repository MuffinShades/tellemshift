#include <iostream>
#include <fstream>
#include <glad.h>
#include "util.hpp"
#include "vec.hpp"
#include "mat.hpp"

class Shader {
public:
    unsigned int PGRM;
	Shader(const char *vertex_data, const char *fragment_data);
	Shader() {

	}
    i32 SetVec2(std::string label, vec2 *v);
    i32 SetVec3(std::string label, vec3 *v);
	i32 SetVec4(std::string label, vec4 *v);
    i32 SetMat4(std::string label, mat4 *m);
	i32 SetFloat(std::string label, f32 v);
	i32 SetInt(std::string label, i32 val);
	i32 SetBool(std::string label, bool b);
	i32 SetiVec2(std::string label, vec2 *v);
	i32 SetiVec3(std::string label, vec3 *v);
	i32 SetiVec4(std::string label, vec4 *v);
	void use();
private:
	void __error_check(unsigned int shader, std::string type) {
		int s = 0;
		char infLog[1024];

		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_LINK_STATUS, &s);

			if (!s) {
				glGetShaderInfoLog(shader, 1024, NULL, infLog);
				std::cout << "Shader Error: " << type << "\n" << infLog << std::endl << std::endl;
			}
		} else {
			glGetProgramiv(shader, GL_LINK_STATUS, &s);

			if (!s) {
				glGetProgramInfoLog(shader, 1024, NULL, infLog);
				std::cout << "Program Error: " << type << "\n" << infLog << std::endl << std::endl;
			}
		}
	}
};