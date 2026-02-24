#ifndef COCKTAIL_RENDERER_TEXTURE_STATICSAMPLER_HPP
#define COCKTAIL_RENDERER_TEXTURE_STATICSAMPLER_HPP

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

#endif // COCKTAIL_RENDERER_TEXTURE_STATICSAMPLER_HPP
