#include "graphics.hpp"
#include "assetManager.hpp"
#include <glad.h>

#define BYTE_TO_CONST_CHAR(b) const_cast<const char*>(reinterpret_cast<char*>((b)))
#define gpu_dynamic_alloc(sz) glBufferData(GL_ARRAY_BUFFER, sz, nullptr, GL_DYNAMIC_DRAW)
#define BATCH_NQUADS 0xfff
#define BATCH_SIZE BATCH_NQUADS * 6

#define vertex_float_attrib(i, s, sz, off) glVertexAttribPointer(i, s, GL_FLOAT, GL_FALSE, sz, (void*)off); \
    glEnableVertexAttribArray(i)

#define FL_SZ 4
#define define_vattrib_struct(i, type, target) \
    glVertexAttribPointer(i, sizeof(type::target) / FL_SZ, GL_FLOAT, GL_FALSE, sizeof(type), (const void*)offsetof(type, target)); \
    glEnableVertexAttribArray(i)

#define vao_create(vao) glGenVertexArrays(1, &vao)

#define vbo_bind(vbo) glBindBuffer(GL_ARRAY_BUFFER, (vbo))

void graphics::Load(std::string vert_a_id, std::string frag_a_id) {
    /**
     * 
     * Load in vertex and fragment shaders
     *
     */
    Asset* def_vert = AssetManager::ReqAsset(vert_a_id),
          *def_frag = AssetManager::ReqAsset(frag_a_id);

    if (def_vert == nullptr || def_frag == nullptr || def_vert->sz <= 0 || def_frag->sz <= 0) {
        std::cout << "Error failed to load default shaders!" << std::endl;
        return;
    }

    std::string vertCode = BYTE_TO_CONST_CHAR(def_vert->bytes);
    _clipStr(vertCode, def_vert->sz);

    std::string fragCode = BYTE_TO_CONST_CHAR(def_frag->bytes);
    _clipStr(fragCode, def_frag->sz);

    std::cout << "----Vert Code----" << std::endl << vertCode << std::endl;
    std::cout << "----Frag Code----" << std::endl << fragCode << std::endl;
    std::cout << std::endl;

    //actually load the shaders
    this->defShader = Shader(
        vertCode.c_str(),
        fragCode.c_str()
    );

    std::cout << "loaded Shaders!" << std::endl;

    std::cout << "Name: " << def_vert->inf.fname << " " << def_vert->inf.fileType << std::endl;

    //delete def_vert, def_frag;


    /*
     *
     * Create need matricies and constants
     * 
     */


    //opengl settings
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << this << std::endl;

    //vertex array
    vao_create(this->vao);
    glBindVertexArray(vao);

    //buffer allocation
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    gpu_dynamic_alloc(BATCH_SIZE * sizeof(Vertex));
    this->vmem_alloc();

    define_vattrib_struct(0, Vertex, posf);
    define_vattrib_struct(1, Vertex, tex);
    define_vattrib_struct(2, Vertex, modColor);
    define_vattrib_struct(3, Vertex, texId);

    //texture buffer
    glGenBuffers(1, &this->texBuffer);
}

#define PROJ_ZMIN -1.0f
#define PROJ_ZMAX  1.0f

//when le window resizes
void graphics::WinResize(const size_t w, const size_t h) {
    this->winW = (float)w;
    this->winH = (float) h;

    glViewport(0.0f, 0.0f, this->winW, this->winH);

    if (this->proj_matrix != nullptr && this->proj_matrix->m != nullptr)
        this->proj_matrix->attemptFree();

    this->proj_matrix = mat4::CreateOrthoProjectionMatrix(
        0.0f, 
        this->winW, 
        this->winH,
        0.0f,
        PROJ_ZMIN, 
        PROJ_ZMAX
    );

    for (size_t i = 0; i < 16; i++) {
        std::cout << "PROJ_MAT: " << this->proj_matrix->glPtr()[i] << std::endl;
    }
}

void graphics::push_verts(Vertex *v, size_t n) {
    if (this->_c_vert + n >= BATCH_SIZE)
        this->render_flush();
    if (v == nullptr || n <= 0)
        return;
    memcpy(this->vmem + this->_c_vert, v, n * sizeof(Vertex));

    //increase current vertex
    this->_c_vert += n;
}

void graphics::vmem_alloc() {
    this->free();
    this->vmem = new Vertex[BATCH_SIZE];
    ZeroMem(this->vmem, BATCH_SIZE);
}

void graphics::vmem_clear() {
    ZeroMem(this->vmem, BATCH_SIZE);
    this->_c_vert = 0;
}

void graphics::texbuffer_popu() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->texBuffer);
    const size_t nTex = this->tex_handles.size();
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        sizeof(GLuint64) * nTex,
        (const void*)this->tex_handles.data(),
        GL_DYNAMIC_COPY
    );
}

void graphics::render_flush() {
    if (this->tex_update) {
        this->texbuffer_popu();
        this->tex_update = false;
    }

    //copy over buffer data to gpu memory
    vbo_bind(this->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * this->_c_vert, (void*)this->vmem);

    //bind to le textures
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->texBuffer);

    //set program variables
    this->defShader.use();
    this->defShader.SetMat4("proj_mat", this->proj_matrix);

    //bind to texture buffer
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->texBuffer);

    //draw stuff
    this->bind_vao();
    glDrawArrays(GL_TRIANGLES, 0, this->_c_vert);

    //clean up
    this->vmem_clear();
    vbo_bind(NULL);
}

const size_t graphics::getEstimatedMemoryUsage() {
    return sizeof(Vertex) * BATCH_SIZE;
}

void graphics::free() {
    safe_free_m(this->vmem);
    safe_free_s(this->proj_matrix);
    this->_c_vert = 0;
}

void graphics::SetFillColor(float r, float g, float b, float a) {
    this->fillColor = {
        r / 255.0f,
        g / 255.0f,
        b / 255.0f,
        a / 255.0f
    };
}

void graphics::FillRect(float x, float y, float w, float h) {
    vec2 p1 = vec2(x + w, y);
	vec2 p2 = vec2(x, y);
	vec2 p3 = vec2(x + w, y + h);
	vec2 p4 = vec2(x, y + h);

    Vertex vertices[] = {
		//triangle 1 (0, 1, 3)
        {p3.x, p3.y, 0.0f,   0.0f, 0.0f,   this->fillColor.x, this->fillColor.y, this->fillColor.z, this->fillColor.w,   -1.0f},
        {p1.x, p1.y, 0.0f,   0.0f, 0.0f,   this->fillColor.x, this->fillColor.y, this->fillColor.z, this->fillColor.w,   -1.0f},
        {p4.x, p4.y, 0.0f,   0.0f, 0.0f,   this->fillColor.x, this->fillColor.y, this->fillColor.z, this->fillColor.w,   -1.0f},

		//triangle 2 (1,2,3)
        {p1.x, p1.y, 0.0f,   0.0f, 0.0f,   this->fillColor.x, this->fillColor.y, this->fillColor.z, this->fillColor.w,   -1.0f},
        {p2.x, p2.y, 0.0f,   0.0f, 0.0f,   this->fillColor.x, this->fillColor.y, this->fillColor.z, this->fillColor.w,   -1.0f},
        {p4.x, p4.y, 0.0f,   0.0f, 0.0f,   this->fillColor.x, this->fillColor.y, this->fillColor.z, this->fillColor.w,   -1.0f}
	};

    this->push_verts(vertices, 6);
}

void graphics::FrameStart() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
}


vec4 graphics::clear_color = { 0.0f, 0.0f, 0.0f, 0.0f };

void graphics::SetClearColor(float r, float g, float b, float a) {
    clear_color = {
        r / 255.0f,
        g / 255.0f,
        b / 255.0f,
        a / 255.0f
    };
}

void graphics::DrawImage(Texture tex, float clipX, float clipY, float clipW, float clipH, float x, float y, float w, float h) {
    if (tex.id < 0) {
        std::cout << "Error could not render texture! Invalid texture id" << std::endl;
        return;
    }

    if (clipW <= 0.0f)
        clipW = tex.w;
    if (clipH <= 0.0f)
        clipH = tex.h;

    vec2 p1 = vec2(x + w, y);
    vec2 p2 = vec2(x, y);
    vec2 p3 = vec2(x + w, y + h);
    vec2 p4 = vec2(x, y + h);

    vec2 tc1 = tex.NormalizeCoord({ clipX + clipW, clipY        }),
         tc2 = tex.NormalizeCoord({ clipX        , clipY        }),
         tc3 = tex.NormalizeCoord({ clipX + clipW, clipY + clipH}),
         tc4 = tex.NormalizeCoord({ clipX        , clipY + clipH});

    const float texId = tex.GetHandle();
    
    tex.fid = (float)tex.id;

    Vertex vertices[] = {
        //triangle 1 (0, 1, 3)
        {p3.x, p3.y, 0.0f,   tc3.x, tc3.y,   0.0f, 0.0f, 0.0f, 0.0f,    tex.fid},
        {p1.x, p1.y, 0.0f,   tc1.x, tc1.y,   0.0f, 0.0f, 0.0f, 0.0f,    tex.fid},
        {p4.x, p4.y, 0.0f,   tc4.x, tc4.y,   0.0f, 0.0f, 0.0f, 0.0f,    tex.fid},

        //triangle 2 (1,2,3)
        {p1.x, p1.y, 0.0f,   tc1.x, tc1.y,   0.0f, 0.0f, 0.0f, 0.0f,    tex.fid},
        {p2.x, p2.y, 0.0f,   tc2.x, tc2.y,   0.0f, 0.0f, 0.0f, 0.0f,    tex.fid},
        {p4.x, p4.y, 0.0f,   tc4.x, tc4.y,   0.0f, 0.0f, 0.0f, 0.0f,    tex.fid}
    };

    this->push_verts(vertices, 6);
}

int graphics::AddTexture(Texture* tex) {
    if (tex == nullptr) return -1;

    int pos = this->tex_handles.size();
    this->tex_handles.push_back(tex->GetHandle());
    this->tex_update = true;
    return pos;
}