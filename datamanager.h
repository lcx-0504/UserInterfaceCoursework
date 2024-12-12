//#ifndef DATAMANAGER_H
//#define DATAMANAGER_H

//#include <QObject>
//#include <QJsonObject>
//#include <QJsonArray>
//#include <QSet>
//#include <QString>
//#include <QFile>

//class DataManager
//{
//public:
//    explicit DataManager(const QString& folderPath);

//    void load();
//    void save();

//    bool getLikeStatus(const QString& videoPath) const;
//    void setLikeStatus(const QString& videoPath, bool status);

//    bool getCollectStatus(const QString& videoPath) const;
//    void setCollectStatus(const QString& videoPath, bool status);

//    QList<QVariantMap> getComments(const QString& videoPath) const;
//    void addComment(const QString& videoPath, const QString& username, const QString& content);
//    void editComment(const QString& videoPath, int commentId, const QString& username, const QString& content);
//    void deleteComment(const QString& videoPath, int commentId);

//private:
//    QString jsonFilePath;
//    QJsonObject videoData;
//    QSet<int> usedCommentIds;

//    void initializeUsedCommentIds();
//    int generateUniqueCommentId();
//    void writeToFile();
//};

//#endif // DATAMANAGER_H


#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QSet>
#include <QString>
#include <QFile>

class DataManager
{
public:
    explicit DataManager(const QString& folderPath);

    void load();
    void save();

    bool getLikeStatus(const QString& videoPath) const;
    void setLikeStatus(const QString& videoPath, bool status);

    bool getCollectStatus(const QString& videoPath) const;
    void setCollectStatus(const QString& videoPath, bool status);

    QList<QVariantMap> getComments(const QString& videoPath) const;
    void addComment(const QString& videoPath, const QString& username, const QString& content);
    void editComment(const QString& videoPath, int commentId, const QString& username, const QString& content);
    void deleteComment(const QString& videoPath, int commentId);

private:
    QString jsonFilePath;
    QJsonObject videoData;
    QSet<int> usedCommentIds;
    QString baseFolderPath;

    void initializeUsedCommentIds();
    int generateUniqueCommentId();
    void writeToFile();

    QString toRelativePath(const QString& absolutePath) const;
    QString toAbsolutePath(const QString& relativePath) const;
};

#endif // DATAMANAGER_H
