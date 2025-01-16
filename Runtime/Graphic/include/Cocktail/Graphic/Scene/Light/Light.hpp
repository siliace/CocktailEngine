#ifndef COCKTAIL_GRAPHIC_SCENE_LIGHT_LIGHT_HPP
#define COCKTAIL_GRAPHIC_SCENE_LIGHT_LIGHT_HPP

#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/Math/Intersection.hpp>
#include <Cocktail/Core/Math/Volume/Frustum.hpp>
#include <Cocktail/Core/Meta/Implements.hpp>

#include <Cocktail/Graphic/Export.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class COCKTAIL_GRAPHIC_API Light : public Implements<Light, Interface>
    {
    public:

        enum class Type
        {
	        Directional,
            Point,
            Spot
        };

        /**
         * \brief 
         * \param frustum 
         * \return 
         */
        virtual Intersection FrustumCull(const Frustum<float>& frustum) const = 0;

        /**
         * \brief 
         * \return 
         */
        virtual Type GetType() const = 0;

        /**
         * \brief 
         * \return 
         */
        virtual LinearColor GetColor() const = 0;

        /**
         * \brief 
         * \return 
         */
        virtual Vector3<float> GetDirection() const = 0;
    };

    struct LightElement
    {
        alignas(16) Light::Type Type;
        alignas(16) LinearColor Color;
        alignas(16) Vector3<float> Position;
        alignas(16) Vector3<float> Direction;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_LIGHT_LIGHT_HPP
