#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_CUSTOMRECORDINFO_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_CUSTOMRECORDINFO_HPP

#include <Cocktail/Graphic/Rendering/Queue/RecordDrawContext.hpp>

namespace Ck
{
    /**
	 * \brief 
	 */
	struct CustomRecordInfo
	{
		/**
		 * \brief 
		 */
		using RenderFunction = void(*)(Renderer::CommandList& commandList, void* userData, RecordDrawContext& drawContext);

		/**
		 * \brief 
		 */
		RenderFunction Draw = nullptr;

		/**
		 * \brief 
		 */
		void* UserData = nullptr;
	};    
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_CUSTOMRECORDINFO_HPP
