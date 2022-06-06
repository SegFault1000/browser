#include "BookmarkModule.h"
#include <QWebEngineView>
#include "widget.h"
#include "MenuHelper.h"
#include <qmenubar.h>
#include <QMenu>
#include <qfile.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
static constexpr const char* const JSON_FILE_NAME = "bookmarks.json";
void BookmarkModule::Init(Widget* w)
{
	this->widget = w;
	w->AddShortcut(QKeySequence("Ctrl+D"), [this] {SaveToBookmarks(); });

	bookmarkMenu = widget->menuBar->addMenu("Bookmarks");	
	LoadBookmarksFromJsonFile();
}

void BookmarkModule::LoadBookmarksFromFile(QFile& file)
{	
	if (!file.isOpen())
		file.open(QIODevice::ReadOnly);
	if (!file.isOpen())
		return;
	QString content = file.readAll();
	file.close();
	
	QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
	if (!doc.isObject())
	{
		qDebug() << "Failed to parse json file.";
		return;
	}

#pragma region reset content
	for (std::pair<const QString, Folder>& kvp : folderMap)
	{
		delete kvp.second.menu;
	}
	folderMap.clear();
	bookmarkMap.clear();
	foldersVector.clear();
	folderMap.clear();
#pragma endregion

	bookmarkObj = doc.object();
	QJsonValueRef objFolders = bookmarkObj["Folders"];
	if (objFolders.isArray())
	{
		QJsonArray foldersArray = objFolders.toArray();
		for (size_t i = 0; i < foldersArray.size(); ++i)
		{			
			QJsonObject entry = foldersArray[i].toObject();
			
			QJsonValueRef folderName = entry["Name"];
			if (!folderName.isString())
			{
				continue;
			}			
			Folder& folder = folderMap[folderName.toString()];
			folder.name = folderName.toString();
			QVector<Bookmark>& bookmarks = folder.bookmarks;			
			QJsonValueRef objBookmarksArray = entry["Bookmarks"];			
			if (!objBookmarksArray.isArray())
			{
				continue;
			}
			QJsonArray bookmarksArray = objBookmarksArray.toArray();
			for (size_t k = 0; k < bookmarksArray.size(); ++k)
			{
				QJsonObject objBookmark = bookmarksArray[k].toObject();
				Bookmark bookmark;
				bookmark.name = objBookmark["Name"].toString();				
				bookmark.url = objBookmark["Url"].toString();
				bookmark.order = objBookmark["Order"].toInt();			
				bookmarks.push_back(std::move(bookmark));
			}
			std::sort(bookmarks.begin(), bookmarks.end(), [](Bookmark& a, Bookmark& b) { return a.order < b.order; });
		}
	}
	QJsonValueRef objBookmarksArray = bookmarkObj["Bookmarks"];
	if (objBookmarksArray.isArray())
	{
		QJsonArray bookmarksArray = objBookmarksArray.toArray();
		for (size_t k = 0; k < bookmarksArray.size(); ++k)
		{
			QJsonObject objBookmark = bookmarksArray[k].toObject();
			Bookmark bookmark;
			bookmark.name = objBookmark["Name"].toString();
			bookmark.url = objBookmark["Url"].toString();
			bookmark.order = objBookmark["Order"].toInt();
			bookmarkMap[bookmark.url] = std::move(bookmark);
		}		
	}
	
	
	foldersVector.reserve(folderMap.size() + 1);
	bookmarksVector.reserve(folderMap.size() + 1);
	for (auto& [name, folder] : folderMap)
		foldersVector.push_back(&folder);
	for (auto& [name, bookmark] : bookmarkMap)
		bookmarksVector.push_back(&bookmark);
	
	auto comp = [](const auto* a, const auto* b) {
		return a->order < b->order;
	};
	std::sort(foldersVector.begin(), foldersVector.end(), comp);
	std::sort(bookmarksVector.begin(), bookmarksVector.end(), comp);	

	for (Folder* folder : foldersVector)
	{
		folder->menu = bookmarkMenu->addMenu(folder->name);		 
		for (const Bookmark& bm : folder->bookmarks)
		{
			MenuHelper_AddAction(folder->menu, bm.name, [this, str = bm.name]
				{
					widget->LoadUrl(str);
				});
		}
	}
	for (const Bookmark* bm : bookmarksVector)
	{
		MenuHelper_AddAction(bookmarkMenu, bm->name, [this, str = bm->name]
			{
				widget->LoadUrl(str);
			});		
	}	
}

void BookmarkModule::LoadBookmarksFromJsonFile()
{
	QFile file(JSON_FILE_NAME);
	LoadBookmarksFromFile(file);
}

void BookmarkModule::SaveToBookmarks()
{		
	QWebEnginePage* page = widget->view->page();
	Bookmark bm;
	bm.url = page->url().toString();
	bm.name = page->title();
	{
		auto it = bookmarkMap.find(bm.url);
		if (it != bookmarkMap.end())
			return;
	}
	bookmarkMap[bm.url] = bm;
	bookmarkMenu->addAction(bm.name, [this, str = bm.url]
		{
			widget->LoadUrl(str);
		});	
	
	if (bookmarkObj["Bookmarks"].isNull() || !bookmarkObj["Bookmarks"].isArray())
		bookmarkObj["Bookmarks"] = QJsonArray();	
	QJsonArray arr = bookmarkObj["Bookmarks"].toArray();
	QJsonObject bmObj;
	bmObj["Name"] = bm.name;
	bmObj["Url"] = bm.url;	
	bmObj["Order"] = (int32_t)bm.order;
	arr.append(bmObj);	
	bookmarkObj["Bookmarks"] = arr;

	QFile file(JSON_FILE_NAME);
	if (file.open(QIODevice::WriteOnly))
	{
		file.write(QJsonDocument(bookmarkObj).toJson());
		file.close();
	}
	
}
