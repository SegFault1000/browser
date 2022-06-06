#include "WebEnginePage.h"
#include "widget.h"

WebEnginePage::WebEnginePage(Widget* widget) : widget(widget)
{
}

bool WebEnginePage::acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame)
{
	if(NavigationType::NavigationTypeLinkClicked && widget->LastMouseButtonPressed() == Widget::MouseButton::Middle)
	{
		//[pending implementation]: code to trigger opening of url in new tab
	}
	return true;
}
