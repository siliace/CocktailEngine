#ifndef COCKTAIL_GRAPHIC_SCENE_LIGHT_LIGHT_HPP
#define COCKTAIL_GRAPHIC_SCENE_LIGHT_LIGHT_HPP

#include <CocktailEngine/Core/Color.hpp>
#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/Math/Intersection.hpp>
#include <CocktailEngine/Core/Math/Volume/Frustum.hpp>

#include <CocktailEngine/Graphic/Export.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class COCKTAILENGINE_GRAPHIC_API Light
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
        LinearColor GetColor() const;

        /**
         * \brief 
         * \param color 
         */
        void SetColor(LinearColor color);

        /**
         * \brief
         * \return
         */
        float GetIntensity() const;

        /**
         * \brief 
         * \param intensity 
         */
        void SetIntensity(float intensity);

        /**
         * \brief
         *
         * \return
         */
        const String& GetName() const;

    protected:

        /**
         * \brief
         *
         * \param color
         * \param intensity
         * \param name
         */
        Light(String name, LinearColor color, float intensity);

    private:

        String mName;
        LinearColor mColor;
        float mIntensity;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_LIGHT_LIGHT_HPP
