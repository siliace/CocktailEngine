#ifndef COCKTAIL_GRAPHIC_SCENE_OBJ_OBJVERTEX_HPP
#define COCKTAIL_GRAPHIC_SCENE_OBJ_OBJVERTEX_HPP

#include <Cocktail/Core/Math/Vector/Vector3.hpp>

namespace Ck
{
	struct ObjVertex
	{
		Vector3<float> Position;
		Vector2<float> TexCoord;
		Vector3<float> Normal;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_OBJ_OBJVERTEX_HPP