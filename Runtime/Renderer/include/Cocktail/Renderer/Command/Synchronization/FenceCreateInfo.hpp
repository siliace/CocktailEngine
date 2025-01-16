#ifndef COCKTAIL_RENDERER_COMMNAD_SYNCHRONIZATION_FENCECREATEINFO_HPP
#define COCKTAIL_RENDERER_COMMNAD_SYNCHRONIZATION_FENCECREATEINFO_HPP

namespace Ck::Renderer
{
	/**
	 * \brief Fence descriptor structure
	 * Describe the set of parameters to create a Fence
	 * \see RenderDevice::CreateFence
	 */
	struct FenceCreateInfo
	{
		/**
		 * \brief Specifies whether the Fence to create must be in a signaled state
		 */
		bool Signaled = false;

		/**
		* \brief Specifies the name of the Fence to create
		* If the extension Debug is not supported by the RenderDevice creating the ShaderProgram, this parameter is ignored
		* \see RenderDeviceObject::GetName
		*/
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_RENDERER_COMMNAD_SYNCHRONIZATION_FENCECREATEINFO_HPP
