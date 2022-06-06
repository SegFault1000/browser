#ifndef INTERCEPTOR_H
#define INTERCEPTOR_H
#include <QWebEngineUrlRequestInterceptor>
#include <QMap>
#include <set>
#include <orderedvector.h>
#include "util.h"
#include <unordered_set>
class Interceptor : public QWebEngineUrlRequestInterceptor
{
private:
  //Maps host names to RequestInfo objects
  using Base = QWebEngineUrlRequestInterceptor;
  QString host;
  QMap<QString, class HostInfo> HostMap;
  std::unordered_set<QString> GlobalBlockList;
public:
  Interceptor(const char* filename = nullptr, QObject* parent = nullptr);
  void interceptRequest(QWebEngineUrlRequestInfo& info) override;
  const QMap<QString, HostInfo>& GetHostMap() const;
  const std::unordered_set<QString>& GetGlobalBlockList() const;
  void SetDataFromFile(const QString& filename);
  void SaveToFile(const QString& path);
private:
  //HostInfo* FindBlockedRequest()
};
class HostInfo
{
public:
  std::unordered_set<QString> BlockList;
  QVector<class QRegularExpression> RegexBlockList;


  
};
#endif // INTERCEPTOR_H
