#ifndef COCKTAILENGINE_RENDERER_TEXTURE_STATICSAMPLER_HPP
#define COCKTAILENGINE_RENDERER_TEXTURE_STATICSAMPLER_HPP

namespace Ck::Renderer
{
	enum class StaticSampler
	{
		None,
		NearestClamp,
		LinearClamp,
		TrilinearClamp,
		NearestWrap,
		LinearWrap,
		TrilinearWrap,
	};
}

#endif // COCKTAILENGINE_RENDERER_TEXTURE_STATICSAMPLER_HPP
