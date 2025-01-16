#ifndef COCKTAIL_RENDERING_ENGINE_SAMPLERTYPE_HPP
#define COCKTAIL_RENDERING_ENGINE_SAMPLERTYPE_HPP

namespace Ck
{
	enum class SamplerType
	{
		NearestClamp,
		LinearClamp,
		TrilinearClamp,
		NearestWrap,
		LinearWrap,
		TrilinearWrap,
	};
}

#endif // COCKTAIL_RENDERING_ENGINE_SAMPLERTYPE_HPP