ck_declare_property(
    MODULE CocktailEngine::Vulkan
    NAME StagingBufferSize
    TYPE INT
    DEFAULT 1048576
    DESCRIPTION "Default size of allocated staging buffers"
)

ck_declare_property(
    MODULE CocktailEngine::Vulkan
    NAME AllocatorChunkSize
    TYPE INT
    DEFAULT 67108864
    DESCRIPTION "Default size of allocated VkDeviceMemory"
)

ck_declare_thirdparty(
    Vulkan-Headers
    PRIVATE
    TARGETS Vulkan::Headers
)

ck_declare_thirdparty(
    volk
    PRIVATE
    TARGETS volk::volk_headers
)

ck_declare_thirdparty(
    SPRIV-Reflect
    PRIVATE
    TARGETS spirv-reflect-static
)

ck_add_shader(
    OUTPUT depth_resolve.vert.spv
    TYPE vert
    FILE depth_resolve.vert
)

ck_add_shader(
    OUTPUT depth_resolve.frag.spv
    TYPE frag
    FILE depth_resolve.frag
)

target_compile_definitions(
    CocktailEngine-Vulkan
    PRIVATE
        VK_NO_PROTOTYPES
        $<$<BOOL:${WIN32}>:VK_USE_PLATFORM_WIN32_KHR>
        $<$<BOOL:${LINUX}>:VK_USE_PLATFORM_XLIB_KHR>
)

if("Xlib" IN_LIST CK_PLATFORM_TAGS)
    find_package(X11 REQUIRED)
    target_link_libraries(
        CocktailEngine-Vulkan
        PRIVATE X11::X11 X11::Xrandr X11::Xcursor
    )
endif()