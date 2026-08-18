#ifndef COCKTAILENGINE_VULKAN_VOLK_HPP
#define COCKTAILENGINE_VULKAN_VOLK_HPP

#include <CocktailEngine/Core/Cocktail.hpp>

#ifdef COCKTAIL_OS_GNU_LINUX
    #include <X11/Xlib.h>
    #include <vulkan/vulkan.h>
    #include <vulkan/vulkan_xlib.h>

    #ifdef Always
        #undef Always
    #endif

    #ifdef None
        #undef None
    #endif
#endif

#include <volk.h>

#endif // COCKTAILENGINE_VULKAN_VOLK_HPP