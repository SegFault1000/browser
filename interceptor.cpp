#include "interceptor.h"
#include <QProcess>
#include <QDebug>
#include <util.h>
#include <QRegularExpression>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
Interceptor::Interceptor(const char* filename, QObject* parent) : Base(parent)
{
  if(filename != nullptr)
  {
    SetDataFromFile(filename);
  }
}

void Interceptor::interceptRequest(QWebEngineUrlRequestInfo& info)
{  
  QString requestUrl = info.requestUrl().toString();
  if(GlobalBlockList.find(requestUrl) != GlobalBlockList.end())
  {
    info.block(true);
    qDebug() << "BLOCKED " << requestUrl << '\n';
    return;
  }

  QString hostName = info.requestUrl().host();
  auto it = HostMap.find(hostName);
  if(it != HostMap.end())
  {
    HostInfo& hostInfo = it.value();
    std::unordered_set<QString>& blocklist = hostInfo.BlockList;
    QVector<QRegularExpression>& regexBlockList = hostInfo.RegexBlockList;

    if(blocklist.find(requestUrl) != blocklist.end())
    {
      info.block(true);
      qDebug() << "BLOCKED " << requestUrl << '\n';
      return;
    }
    const static auto regexLambda = [&](const QRegularExpression& exp){
      return exp.match(requestUrl).hasMatch();
    };
    if(util::contains_if(regexBlockList.begin(), regexBlockList.end(), regexLambda))
    {
      info.block(true);
      return;
    }
  }


}

const QMap<QString, HostInfo>& Interceptor::GetHostMap() const
{
  return HostMap;
}

const std::unordered_set<QString>& Interceptor::GetGlobalBlockList() const
{
  return GlobalBlockList;
}

void Interceptor::SetDataFromFile(const QString& filename)
{
  QFile file(filename);
  GlobalBlockList.clear();
  HostMap.clear();

  if(!file.open(QIODevice::ReadOnly))
  {
    file.open(QIODevice::WriteOnly);
    file.close();
  }
  else
  {
    QString fileContent = file.readAll().trimmed();
    file.close();
    if(fileContent.size() == 0)
    {
      return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(fileContent.toUtf8());
    //fill GlobalBlockList
    QJsonValue GlobalBlockListJson = doc["GlobalBlockList"];
    if(GlobalBlockListJson.isArray())
    {
      for(QJsonValueRef obj : doc["GlobalBlockList"].toArray())
      {
        if(obj.isString())
        {
          GlobalBlockList.insert(obj.toString());
        }
      }
    }

    //fill HostMap
    QJsonValue HostMapJson = doc["HostMap"];
    if(HostMapJson.isArray())
    {
      for(QJsonValueRef valRef : HostMapJson.toArray())
      {
        QJsonObject obj = valRef.toObject();
        QString hostName = obj["Name"].toString();
        HostInfo hostInfo;
        QJsonValue objBlockList = obj["BlockList"];
        if(objBlockList.isArray())
          for(QJsonValueRef x : objBlockList.toArray())
            if(x.isString())
              hostInfo.BlockList.insert(x.toString());

        QJsonValue objRegexBlockList = obj["RegexBlockList"];
        if(objRegexBlockList.isArray())
          for(QJsonValueRef x : objRegexBlockList.toArray())
            if(x.isString())
              hostInfo.RegexBlockList.push_back(QRegularExpression(x.toString()));
      }
    }
  }

}

void Interceptor::SaveToFile(const QString& path)
{


}


