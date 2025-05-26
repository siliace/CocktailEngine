#ifndef COCKTAIL_GRAPHIC_SCENE_LIGHT_LIGHT_HPP
#define COCKTAIL_GRAPHIC_SCENE_LIGHT_LIGHT_HPP

#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/Math/Intersection.hpp>
#include <Cocktail/Core/Math/Volume/Frustum.hpp>

#include <Cocktail/Graphic/Export.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class COCKTAIL_GRAPHIC_API Light
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
         */
        virtual ~Light() = default;

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
         * \param color 
         */
        virtual void SetColor(LinearColor color) = 0;

        /**
         * \brief
         * \return
         */
        virtual float GetIntensity() const = 0;

        /**
         * \brief 
         * \param intensity 
         */
        virtual void SetIntensity(float intensity) = 0;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_LIGHT_LIGHT_HPP
