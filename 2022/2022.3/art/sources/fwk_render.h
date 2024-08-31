// -----------------------------------------------------------------------------
// naive rendering framework
// - rlyeh, public domain
//
// IQM skeletal meshes by @lsalzman (public domain) - https://bit.ly/2OQh0Me
// SH code by @ands (public domain) - https://github.com/ands/spherical_harmonics_playground
// SHM code by @jarikomppa (unlicensed) - https://github.com/jarikomppa/shadertoolkit

typedef unsigned handle; // GLuint

// -----------------------------------------------------------------------------
// colors

API uint32_t rgba( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
API uint32_t bgra( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
API float    alpha( uint32_t rgba );

#define RGBX(rgb,x)   ( ((rgb)&0xFFFFFF) | (((unsigned)(x))<<24) )
#define RGB3(r,g,b)   ( ((r)<<16) | ((g)<<8) | (b) )
#define RGB4(r,g,b,a) RGBX(RGB3(r,g,b),a)

#define BLACK   RGBX(0x000000,255)
#define RED     RGBX(0xFF004D,255)
#define GREEN   RGBX(0x00B543,255)
#define BLUE    RGBX(0x065AB5,255)
#define ORANGE  RGBX(0xFF6C24,255)
#define CYAN    RGBX(0x29ADFF,255)
#define PURPLE  RGBX(0x7E2553,255)
#define YELLOW  RGBX(0xFFEC27,255)
#define WHITE   RGBX(0xFFF1E8,255)
#define GRAY    RGBX(0x725158,255)

// -----------------------------------------------------------------------------
// images

/// flags when constructing the image_t type. see: image, image_from_mem
/// IMAGE_R: 1-channel image (R)
/// IMAGE_RG: 2-channel image (R,G)
/// IMAGE_RGB: 3-channel image (R,G,B)
/// IMAGE_RGBA: 4-channel image (R,G,B,A)
/// IMAGE_FLIP: Flip image vertically
/// IMAGE_FLOAT: Float pixel components
enum IMAGE_FLAGS {
    IMAGE_R     = 0x01000,
    IMAGE_RG    = 0x02000,
    IMAGE_RGB   = 0x04000,
    IMAGE_RGBA  = 0x08000,
    IMAGE_FLIP  = 0x10000,
    IMAGE_FLOAT = 0x20000,
};

/// type that holds linear uncompressed bitmap of any given dimensions.
/// w,h: image dimensions in pixels. `x,y` alias.
/// comps: number of components per pixel. `n` alias.
/// pixels: untyped pointer to linear bitmap data. typed pointers use `pixels8/16/32/f` aliases.
/// see: texture_t
typedef struct image_t {
    union { unsigned x, w; };
    union { unsigned y, h; };
    union { unsigned n, comps; };
    union { void *pixels; uint8_t *pixels8; uint16_t *pixels16; uint32_t *pixels32; float *pixelsf; };
} image_t;

API image_t image(const char *pathfile, int flags);
API image_t image_from_mem(const char *ptr, int len, int flags);
API void    image_destroy(image_t *img);

// -----------------------------------------------------------------------------
// textures

enum TEXTURE_FLAGS {
    // UNIT[0..7]

    TEXTURE_BC1 = 8,  // DXT1, RGB with 8:1 compression ratio (+ optional 1bpp for alpha)
    TEXTURE_BC2 = 16, // DXT3, RGBA with 4:1 compression ratio (BC1 for RGB + 4bpp for alpha)
    TEXTURE_BC3 = 32, // DXT5, RGBA with 4:1 compression ratio (BC1 for RGB + BC4 for A)
//  TEXTURE_BC4,     // Alpha

    TEXTURE_NEAREST = 0,
    TEXTURE_LINEAR = 64,
    TEXTURE_MIPMAPS = 128,

    TEXTURE_CLAMP = 0,
    TEXTURE_BORDER = 0x100,
    TEXTURE_REPEAT = 0x200,

    TEXTURE_BYTE = 0,
    TEXTURE_FLOAT = IMAGE_FLOAT,

    TEXTURE_COLOR = 0,
    TEXTURE_DEPTH = 0x800,

    TEXTURE_R = IMAGE_R,
    TEXTURE_RG = IMAGE_RG,
    TEXTURE_RGB = IMAGE_RGB,
    TEXTURE_RGBA = IMAGE_RGBA,
    TEXTURE_FLIP = IMAGE_FLIP,

    // @fixme
    TEXTURE_SRGB = 1 << 24,
    TEXTURE_BGR = 1 << 25,
    TEXTURE_ARRAY = 1 << 26,
};

typedef struct texture_t {
    union { unsigned x, w; };
    union { unsigned y, h; };
    union { unsigned z, d; };
    union { unsigned n, bpp; };
    handle id, unit;
    unsigned flags;
    char* filename;
    bool transparent;
} texture_t;

API texture_t texture_compressed(const char *filename, unsigned flags);
API texture_t texture_compressed_from_mem(const void *data, int len, unsigned flags);

API texture_t texture(const char* filename, int flags);
API texture_t texture_from_mem(const char* ptr, int len, int flags);
API texture_t texture_create(unsigned w, unsigned h, unsigned n, void *pixels, int flags);
API texture_t texture_checker();
API void      texture_destroy(texture_t *t);
// textureLod(filename, dir, lod);
// void texture_add_loader( int(*loader)(const char *filename, int *w, int *h, int *bpp, int reqbpp, int flags) );
API unsigned  texture_update(texture_t *t, unsigned w, unsigned h, unsigned n, void *pixels, int flags);

// -----------------------------------------------------------------------------
// brdf

API texture_t brdf_lut();

// -----------------------------------------------------------------------------
// pbr materials

typedef struct colormap_t {
    vec4 color;
    texture_t *texture;
} colormap_t;

API bool colormap( colormap_t *cm, const char *pbr_material_type, bool load_as_srgb );

typedef struct pbr_material_t {
    char* name;
    colormap_t diffuse;
    colormap_t normals;
    colormap_t specular;
    colormap_t albedo;
    colormap_t roughness;
    colormap_t metallic;
    colormap_t ao;
    colormap_t ambient;
    colormap_t emissive;

    float specular_shininess;
} pbr_material_t;

API bool pbr_material(pbr_material_t *pbr, const char *material);
API void pbr_material_destroy(pbr_material_t *m);

// -----------------------------------------------------------------------------
// fullscreen quads

API void  fullscreen_rgb_quad( texture_t texture_rgb, float gamma );
API void  fullscreen_ycbcr_quad( texture_t texture_YCbCr[3], float gamma );

// -----------------------------------------------------------------------------
// sprites

// texture id, position(x,y,depth sort), tint color, rotation angle
API void tile( texture_t texture, float position[3], float rotation /*0*/, uint32_t color /*~0u*/);

// texture id, position(x,y,depth sort), rotation angle, offset(x,y), scale(x,y), is_additive, tint color, spritesheet(frameNumber,X,Y) (frame in a X*Y spritesheet)
API void sprite( texture_t texture, float position[3], float rotation, float offset[2], float scale[2], int is_additive, uint32_t rgba, float spritesheet[3]);

API void sprite_flush();

// -----------------------------------------------------------------------------
// cubemaps

typedef struct cubemap_t {
    unsigned id;    // texture id
    vec3 sh[9];     // precomputed spherical harmonics coefficients
} cubemap_t;

API cubemap_t  cubemap( const image_t image, int flags ); // 1 equirectangular panorama
API cubemap_t  cubemap6( const image_t images[6], int flags ); // 6 cubemap faces
API void       cubemap_destroy(cubemap_t *c);
API cubemap_t* cubemap_get_active();

// -----------------------------------------------------------------------------
// fbos

API unsigned fbo( unsigned texture_color, unsigned texture_depth, int wr_flags );
API void     fbo_bind(unsigned id);
API void     fbo_unbind();
API void     fbo_destroy(unsigned id);

// -----------------------------------------------------------------------------
// shadowmaps

typedef struct shadowmap_t {
    mat44  shadowmatrix;
    mat44  mvp;
    mat44  mv;
    mat44  proj;
    vec4   light_position;
    int    saved_fb;
    int    saved_viewport[4];
    handle fbo, texture;
    int texture_width;
} shadowmap_t;

API shadowmap_t shadowmap(int texture_width); // = 1024
API void shadowmap_destroy(shadowmap_t *s);

API void shadowmap_set_shadowmatrix(shadowmap_t *s, vec3 aLightPos, vec3 aLightAt, vec3 aLightUp, const mat44 projection);
API void shadowmap_begin(shadowmap_t *s);
API void shadowmap_end(shadowmap_t *s);

// shadowmap utils

API void shadowmatrix_proj(mat44 shm_proj, float aLightFov, float znear, float zfar);
API void shadowmatrix_ortho(mat44 shm_proj, float left, float right, float bottom, float top, float znear, float zfar);

// -----------------------------------------------------------------------------
// shaders

API unsigned shader(const char *vs, const char *fs, const char *attribs, const char *fragcolor);
API unsigned shader_bind(unsigned program);
API     void shader_bool(const char *uniform, bool i );
API     void shader_int(const char *uniform, int i);
API     void shader_uint(const char *uniform, unsigned i );
API     void shader_float(const char *uniform, float f);
API     void shader_vec2(const char *uniform, vec2 v);
API     void shader_vec3(const char *uniform, vec3 v);
API     void shader_vec4(const char *uniform, vec4 v);
API     void shader_mat44(const char *uniform, mat44 m);
API     void shader_texture(const char *sampler, texture_t texture);
API     void shader_texture_unit(const char *sampler, unsigned texture, unsigned unit);
API     void shader_colormap(const char *name, colormap_t cm);
API unsigned shader_get_active();
API void     shader_destroy(unsigned shader);


// -----------------------------------------------------------------------------
// meshes (@fixme: deprecate?)

enum MESH_FLAGS {
    MESH_STATIC = 0, // STATIC, DYNAMIC, STREAM // zero|single|many updates per frame
    MESH_STREAM = 1,
    MESH_TRIANGLE_STRIP = 2,
};

typedef struct mesh_t {
    handle vao, vbo, ibo;
    unsigned vertex_count;
    unsigned index_count;
    unsigned flags;
} mesh_t;

API mesh_t mesh_create(const char *format, int vertex_stride,int vertex_count,const void *interleaved_vertex_data, int index_count,const void *index_data, int flags);
API   void mesh_upgrade(mesh_t *m, const char *format, int vertex_stride,int vertex_count,const void *interleaved_vertex_data, int index_count,const void *index_data, int flags);
API   void mesh_render(mesh_t *m);
API   void mesh_destroy(mesh_t *m);
API   aabb mesh_bounds(mesh_t *m);

// -----------------------------------------------------------------------------
// materials

enum MATERIAL_ENUMS {
    MAX_CHANNELS_PER_MATERIAL = 8
};

typedef struct material_t {
    char *name;

    int count;
    struct material_layer_t {
        char   texname[32];
        handle texture;
        float  value;
        vec4   color; // uint32_t
    };
    struct material_layer_t layer[MAX_CHANNELS_PER_MATERIAL];

} material_t;

// -----------------------------------------------------------------------------
// models

enum MODEL_FLAGS {
    MODEL_NO_ANIMATIONS = 1,
    MODEL_NO_MESHES = 2,
    MODEL_NO_TEXTURES = 4,
    MODEL_MATCAPS = 8,
};

typedef struct model_t {
    struct iqm_t *iqm; // private

    unsigned num_textures;
    handle *textures;
    char **texture_names;
    array(material_t) materials;

    unsigned num_meshes;
    unsigned num_triangles;
    unsigned num_joints; // num_poses;
    unsigned num_anims;
    unsigned num_frames;
    handle program;
    float curframe;
    mat44 pivot;

    int stride; // usually 60 bytes (12*4+4*3) for a p3 u2 n3 t4 i4B w4B c4B vertex stream
    void *verts;
    int num_verts;
    handle vao, ibo, vbo, vao_instanced;

    unsigned flags;
    unsigned billboard;

    float *instanced_matrices;
    unsigned num_instances;
} model_t;

API model_t  model(const char *filename, int flags);
API model_t  model_from_mem(const void *mem, int sz, int flags);
API float    model_animate(model_t, float curframe);
API float    model_animate_clip(model_t, float curframe, int minframe, int maxframe, bool loop);
API aabb     model_aabb(model_t, mat44 transform);
API void     model_render(model_t, mat44 proj, mat44 view, mat44 model, int shader);
API void     model_render_skeleton(model_t, mat44 model);
API void     model_render_instanced(model_t, mat44 proj, mat44 view, mat44 *models, int shader, unsigned count);
API void     model_set_texture(model_t, texture_t t);
API bool     model_get_bone_pose(model_t m, unsigned joint, mat34 *out);
API void     model_destroy(model_t);

// -----------------------------------------------------------------------------
// skyboxes

typedef struct skybox_t {
    handle program;
    mesh_t geometry;
    cubemap_t cubemap;
    int flags;
} skybox_t;

API skybox_t skybox(const char *panorama_or_cubemap_folder, int flags);
API int      skybox_push_state(skybox_t *sky, mat44 proj, mat44 view);
API int      skybox_pop_state();
API void     skybox_destroy(skybox_t *sky);

// -----------------------------------------------------------------------------
// post-fxs

API void     viewport_color(uint32_t color); // background(uint32_t) instead?
API void     viewport_color3(vec3 color); // background3(vec3) instead?
API void     viewport_clear(bool color, bool depth);
API void     viewport_clip(vec2 from, vec2 to);

API void     fx_load(const char *file);
API void     fx_begin();
API void     fx_end();
API void     fx_enable(int pass, int enabled);
API int      fx_enabled(int pass);
API void     fx_enable_all(int enabled);
API char *   fx_name(int pass);

// -----------------------------------------------------------------------------
// utils

API void*    screenshot(unsigned components); // 3 RGB, 4 RGBA, -3 BGR, -4 BGRA
