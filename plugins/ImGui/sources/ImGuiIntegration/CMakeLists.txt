ck_declare_thirdparty(
	imgui
	PUBLIC
	TARGETS ImGui
)

ck_add_shader(
	OUTPUT default.vert.spv
	TYPE vert
	FILE default.vert
)

ck_add_shader(
	OUTPUT default.frag.spv
	TYPE frag
	FILE default.frag
)
