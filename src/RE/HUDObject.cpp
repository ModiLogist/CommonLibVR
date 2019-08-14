#include "RE/HUDObject.h"

#include "RE/FxDelegateArgs.h"  // FxDelegateArgs
#include "RE/GFxMovieView.h"  // GFxMovieView


namespace RE
{
	HUDObject::HUDObject(GFxMovieView* a_view) :
		view(a_view),
		root()
	{}


	HUDObject::~HUDObject()
	{}


	bool HUDObject::ProcessMessage(UIMessage* a_message)
	{
		return false;
	}


	void HUDObject::RegisterHUDComponent(FxDelegateArgs& a_params)
	{
		root = a_params[HudComponent::kThis];
	}


	void HUDObject::CleanUp()
	{
		return;
	}
}
