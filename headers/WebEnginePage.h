#pragma once
#include <qwebenginepage.h>
class WebEnginePage : public QWebEnginePage
{
	class Widget* widget = nullptr;
	WebEnginePage(Widget* widget);
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame) override;
};