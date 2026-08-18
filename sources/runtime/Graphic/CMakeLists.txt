ck_declare_thirdparty(
    tinygltf
    PRIVATE
    TARGETS tinygltf
)

ck_declare_thirdparty(
	tinyobjloader
    PRIVATE
	TARGETS tinyobjloader
)

ck_add_shader(
    OUTPUT shaders/line/basic/default.vert.spv
    TYPE vert
    FILE shaders/line/basic_line.vert
)

ck_add_shader(
    OUTPUT shaders/line/basic/default.frag.spv
    TYPE frag
    FILE shaders/line/basic_line.frag
)

ck_add_shader(
    OUTPUT shaders/line/basic/colored.vert.spv
    TYPE vert
    FILE shaders/line/basic_line.vert
    MACRO COCKTAIL_VERTEX_HAS_COLOR
)

ck_add_shader(
    OUTPUT shaders/line/basic/colored.frag.spv
    TYPE frag
    FILE shaders/line/basic_line.frag
    MACRO COCKTAIL_VERTEX_HAS_COLOR
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/default-no-normal.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/basic_mesh.vert
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/default-no-normal.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/basic_mesh.frag
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/default.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/basic_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/default.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/basic_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/colored.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/basic_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_COLOR COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/colored.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/basic_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_COLOR COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/textured-no-normal.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/basic_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/textured-no-normal.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/basic_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/textured.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/basic_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/textured.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/basic_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/textured-pbr.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/basic_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL COCKTAIL_VERTEX_HAS_TANGENT COCKTAIL_MATERIAL_HAS_NORMAL_TEXTURE COCKTAIL_MATERIAL_HAS_PBR_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/textured-pbr.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/basic_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL COCKTAIL_VERTEX_HAS_TANGENT COCKTAIL_MATERIAL_HAS_NORMAL_TEXTURE COCKTAIL_MATERIAL_HAS_PBR_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/textured-blended.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/basic_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/basic/textured-blended.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/basic_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/basic/default.vert.spv
    TYPE vert
    FILE shaders/mesh/unlit/basic_mesh.vert
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/basic/default.frag.spv
    TYPE frag
    FILE shaders/mesh/unlit/basic_mesh.frag
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/basic/colored.vert.spv
    TYPE vert
    FILE shaders/mesh/unlit/basic_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_COLOR
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/basic/colored.frag.spv
    TYPE frag
    FILE shaders/mesh/unlit/basic_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_COLOR
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/basic/textured.vert.spv
    TYPE vert
    FILE shaders/mesh/unlit/basic_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/basic/textured.frag.spv
    TYPE frag
    FILE shaders/mesh/unlit/basic_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/basic/textured-blended.vert.spv
    TYPE vert
    FILE shaders/mesh/unlit/basic_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/basic/textured-blended.frag.spv
    TYPE frag
    FILE shaders/mesh/unlit/basic_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/default-no-normal.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/instanced_mesh.vert
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/default-no-normal.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/instanced_mesh.frag
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/default.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/instanced_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/default.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/instanced_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/colored.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/instanced_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_COLOR COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/colored.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/instanced_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_COLOR COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/textured.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/instanced_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/textured.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/instanced_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/textured-pbr.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/instanced_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL COCKTAIL_VERTEX_HAS_TANGENT COCKTAIL_MATERIAL_HAS_NORMAL_TEXTURE COCKTAIL_MATERIAL_HAS_PBR_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/textured-pbr.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/instanced_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL COCKTAIL_VERTEX_HAS_TANGENT COCKTAIL_MATERIAL_HAS_NORMAL_TEXTURE COCKTAIL_MATERIAL_HAS_PBR_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/textured-blended.vert.spv
    TYPE vert
    FILE shaders/mesh/lit/instanced_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/lit/instanced/textured-blended.frag.spv
    TYPE frag
    FILE shaders/mesh/lit/instanced_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE COCKTAIL_VERTEX_HAS_NORMAL
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/instanced/default.vert.spv
    TYPE vert
    FILE shaders/mesh/unlit/instanced_mesh.vert
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/instanced/default.frag.spv
    TYPE frag
    FILE shaders/mesh/unlit/instanced_mesh.frag
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/instanced/colored.vert.spv
    TYPE vert
    FILE shaders/mesh/unlit/instanced_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_COLOR
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/instanced/colored.frag.spv
    TYPE frag
    FILE shaders/mesh/unlit/instanced_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_COLOR
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/instanced/textured.vert.spv
    TYPE vert
    FILE shaders/mesh/unlit/instanced_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/instanced/textured.frag.spv
    TYPE frag
    FILE shaders/mesh/unlit/instanced_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/instanced/textured-blended.vert.spv
    TYPE vert
    FILE shaders/mesh/unlit/instanced_mesh.vert
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE
)

ck_add_shader(
    OUTPUT shaders/mesh/unlit/instanced/textured-blended.frag.spv
    TYPE frag
    FILE shaders/mesh/unlit/instanced_mesh.frag
    MACRO COCKTAIL_VERTEX_HAS_UV COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE
)

target_compile_definitions(
	CocktailEngine-Graphic
	PRIVATE TINYGLTF_NO_STB_IMAGE TINYGLTF_NO_STB_IMAGE_WRITE
)