#pragma once

//phosphoxybdium graphics engine (or maybe moloxy or phosphoxy or molophos

#include <iostream>
#include "window.hpp"
#include "Shader.hpp"
#include "texture.hpp"

struct Vertex {
    float posf[3];
    float tex[2];
    float modColor[4];
    float texId = -1.0f;
};

class graphics {
private:
    Window *win = nullptr;
    mat4 *proj_matrix = nullptr;
    size_t _c_vert = 0;
    Vertex *vmem = nullptr;
    void vmem_alloc();
    void vmem_clear();
    void push_verts(Vertex *v, size_t n);
    void texbuffer_popu();
    bool tex_update = true;
    void bind_vao() {
        if (this->vao != 0)
            glBindVertexArray(this->vao);
    }
    static vec4 clear_color;
    GLuint texBuffer = 0;
    std::vector<GLuint64> tex_handles;
public:
    Shader defShader;
    float winW, winH;
    u32 vao = 0, vbo = 0;
    vec4 fillColor = {0,0,0,0};
    graphics(Window *w) {
        if (w != nullptr)
            this->win = w;
        else
            return;

        this->winW = this->win->w;
        this->winH = this->win->h;
    }
    void Load(std::string vert_a_id, std::string frag_a_id);
    void WinResize(const size_t w, const size_t h);
    void render_flush();
    void DrawImage(Texture tex, float clipX, float clipY, float clipW, float clipH, float x, float y, float w, float h);
    void FillRect(float x, float y, float w, float h);
    void SetFillColor(float r, float g, float b, float a = 255.0f);
    void free();
    int AddTexture(Texture* tex);
    static void FrameStart();
    static void SetClearColor(float r, float g, float b, float a = 255.0f);
    const size_t getEstimatedMemoryUsage();
};