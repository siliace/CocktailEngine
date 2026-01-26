#ifndef COCKAIL_GRAPHIC_MESH_MESHFACTORY_HPP
#define COCKAIL_GRAPHIC_MESH_MESHFACTORY_HPP

#include <Cocktail/Core/Color.hpp>

#include <Cocktail/Graphic/Mesh/Mesh.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API MeshFactory
	{
	public:

		/**
		 * \brief Create a colored cube mesh
		 * \param size The size of the cube's edge to create
		 * \param color The color of the cube to create
		 * \return The mesh of the cube
		 */
		static std::shared_ptr<Mesh> CreateCube(float size, const LinearColor& color = LinearColor::Black);

		/**
		 * \brief 
		 * \param box 
		 * \param color 
		 * \return 
		 */
		static std::shared_ptr<Mesh> CreateBox(const Box<float>& box, const LinearColor& color = LinearColor::Black);
	};
}

#endif // COCKAIL_GRAPHIC_MESH_MESHFACTORY_HPP
