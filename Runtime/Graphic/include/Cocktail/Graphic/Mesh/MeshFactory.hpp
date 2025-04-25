#ifndef COCKAIL_GRAPHIC_MESH_MESHFACTORY_HPP
#define COCKAIL_GRAPHIC_MESH_MESHFACTORY_HPP

#include <Cocktail/Graphic/Mesh/Mesh.hpp>

#include "Cocktail/Core/Color.hpp"

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API MeshFactory
	{
	public:

		/**
		 * \brief 
		 * \param box 
		 * \param color 
		 * \return 
		 */
		static Ref<Mesh> CreateBox(const Box<float>& box, const LinearColor& color = LinearColor(0.f, 0.f, 0.f, 1.f));
	};
}

#endif // COCKAIL_GRAPHIC_MESH_MESHFACTORY_HPP
