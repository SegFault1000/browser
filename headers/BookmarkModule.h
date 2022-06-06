#pragma once
#include "BrowserModule.h"
#include <unordered_map>
#include <QString>
#include <qjsondocument.h>
#include <QjsonObject.h>
#include <QVector.h>
class QMenu;
class BookmarkModule : public BrowserModule
{
	struct Bookmark
	{
		QString url;
		QString name;
		int32_t order = 0;					
	};
	
	struct Folder
	{
		QString name;	
		QVector<Bookmark> bookmarks;
		int32_t order = 0;
		class QMenu* menu = nullptr;
	};
	QVector<Bookmark> bookmarks;

	Widget* widget = nullptr;
	QMenu* bookmarkMenu = nullptr;
	QJsonDocument bookmarkDoc;
	QJsonObject bookmarkObj;
	std::unordered_map<QString, Folder> folderMap;
	std::unordered_map<QString, Bookmark> bookmarkMap;
	QVector<Folder*> foldersVector;
	QVector<Bookmark*> bookmarksVector;
public:
	void Init(Widget*) override;
	void LoadBookmarksFromFile(class QFile& file);
	void LoadBookmarksFromJsonFile();
public:
	void SaveToBookmarks();
};