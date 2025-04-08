#ifndef IMGUISERVICEPROVIDER_HPP
#define IMGUISERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/System/Window/Window.hpp>

class ImGuiServiceProvider : public Ck::ServiceProvider
{
public:

	/**
	 * \brief
	 * \param application
	 */
	explicit ImGuiServiceProvider(Ck::Application* application) :
		ServiceProvider(application)
	{
		/// Nothing
	}

protected:

	void DoBoot(Ck::Application* application) override;

private:

	void ConnectWindowEvents(Ck::Window& window);

	Ck::Ref<Ck::Window> mEventWindow;
};

#endif // IMGUISERVICEPROVIDER_HPP
