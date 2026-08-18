#ifndef COCKTAILENGINE_RENDERER_SHADER_DESCRIPTORTYPE_HPP
#define COCKTAILENGINE_RENDERER_SHADER_DESCRIPTORTYPE_HPP

namespace Ck::Renderer
{
	/**
	 * \brief Enumeration describing how a resource will be used on a shader
	 */
	enum class DescriptorType
	{
		/**
		 * \brief The resource is a sampler
		 */
		Sampler,

		/**
		 * \brief The resource is a combined sampler and texture view
		 */
		TextureSampler,

		/**
		 * \brief The resource is a texture view to be sampled
		 */
		Texture,

		/**
		 * \brief The resource is accessed like an image
		 */
		StorageTexture,

		/**
		 * \brief
		 */
		UniformBuffer,

		/**
		 * \brief 
		 */
		StorageBuffer,
	};
}

#endif // COCKTAILENGINE_RENDERER_SHADER_DESCRIPTORTYPE_HPP
