//#include "datamanager.h"
//#include <QJsonDocument>
//#include <QVariantMap>
//#include <QVariantList>
//#include <QFile>
//#include <QDebug>

//DataManager::DataManager(const QString& folderPath)
//    : jsonFilePath(folderPath + "/data.json")
//{
//    load();
//}

//void DataManager::load() {
//    QFile file(jsonFilePath);
//    if (file.exists() && file.open(QIODevice::ReadOnly)) {
//        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
//        videoData = doc.object();
//        file.close();
//    } else {
//        // 初始化一个空的 JSON 对象
//        videoData = QJsonObject();
//    }

//    // 初始化已使用的评论 ID 集合
//    initializeUsedCommentIds();
//}

//void DataManager::save() {
//    writeToFile();
//}

//bool DataManager::getLikeStatus(const QString& videoPath) const {
//    if (!videoData.contains(videoPath)) {
//        return false; // 默认未点赞
//    }
//    return videoData[videoPath].toObject().value("liked").toBool(false);
//}


//void DataManager::setLikeStatus(const QString& videoPath, bool status) {
//    QJsonObject video = videoData[videoPath].toObject(); // 确保存在
//    video["liked"] = status;
//    videoData[videoPath] = video;
//    writeToFile();
//}

//bool DataManager::getCollectStatus(const QString& videoPath) const {
//    // 如果路径不存在，返回默认值
//    if (!videoData.contains(videoPath)) {
//        return false;
//    }
//    return videoData[videoPath].toObject().value("collected").toBool(false);
//}

//void DataManager::setCollectStatus(const QString& videoPath, bool status) {
//    QJsonObject video = videoData[videoPath].toObject(); // 确保存在
//    video["collected"] = status;
//    videoData[videoPath] = video;
//    writeToFile();
//}

//QList<QVariantMap> DataManager::getComments(const QString& videoPath) const {
//    // 如果路径不存在，返回空列表
//    if (!videoData.contains(videoPath)) {
//        return {};
//    }
//    QJsonArray comments = videoData[videoPath].toObject().value("comments").toArray();
//    QList<QVariantMap> result;
//    for (const QJsonValue& comment : comments) {
//        result.append(comment.toObject().toVariantMap());
//    }
//    return result;
//}

//void DataManager::addComment(const QString& videoPath, const QString& username, const QString& content) {
//    QJsonObject video = videoData[videoPath].toObject(); // 确保存在
//    QJsonArray comments = video.value("comments").toArray();

//    QJsonObject newComment;
//    newComment["id"] = generateUniqueCommentId(); // 生成唯一评论 ID
//    newComment["username"] = username;
//    newComment["content"] = content;

//    comments.append(newComment);
//    video["comments"] = comments;
//    videoData[videoPath] = video;

//    writeToFile(); // 保存到 JSON 文件
//}


//void DataManager::editComment(const QString& videoPath, int commentId, const QString& newUsername, const QString& newContent) {
//    QJsonObject video = videoData[videoPath].toObject(); // 确保存在
//    QJsonArray comments = video.value("comments").toArray();

//    for (int i = 0; i < comments.size(); ++i) {
//        QJsonObject comment = comments[i].toObject();
//        if (comment["id"].toInt() == commentId) {
//            comment["username"] = newUsername;
//            comment["content"] = newContent;
//            comments[i] = comment;
//            break;
//        }
//    }
//    video["comments"] = comments;
//    videoData[videoPath] = video;

//    writeToFile();
//}

//void DataManager::deleteComment(const QString& videoPath, int commentId) {
//    QJsonObject video = videoData[videoPath].toObject(); // 确保存在
//    QJsonArray comments = video.value("comments").toArray();

//    for (int i = 0; i < comments.size(); ++i) {
//        if (comments[i].toObject()["id"].toInt() == commentId) {
//            comments.removeAt(i);
//            break;
//        }
//    }
//    video["comments"] = comments;
//    videoData[videoPath] = video;

//    writeToFile();
//}

//void DataManager::initializeUsedCommentIds() {
//    usedCommentIds.clear();
//    for (const QString& videoPath : videoData.keys()) {
//        QJsonArray comments = videoData[videoPath].toObject().value("comments").toArray();
//        for (const QJsonValue& comment : comments) {
//            int id = comment.toObject().value("id").toInt();
//            usedCommentIds.insert(id);
//        }
//    }
//}

//int DataManager::generateUniqueCommentId() {
//    static int counter = 1;

//    // 确保 ID 不重复
//    while (usedCommentIds.contains(counter)) {
//        ++counter;
//    }
//    usedCommentIds.insert(counter);
//    return counter;
//}

//void DataManager::writeToFile() {
//    QFile file(jsonFilePath);
//    if (file.open(QIODevice::WriteOnly)) {
//        QJsonDocument doc(videoData);
//        file.write(doc.toJson());
//        file.close();
//    }
//}

#include "datamanager.h"
#include <QJsonDocument>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

DataManager::DataManager(const QString& folderPath)
    : jsonFilePath(folderPath + "/data.json"),
      baseFolderPath(folderPath)
{
    load();
}

void DataManager::load() {
    QFile file(jsonFilePath);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        videoData = doc.object();
        file.close();
    } else {
        videoData = QJsonObject(); // 初始化为空
    }
    initializeUsedCommentIds();
}

void DataManager::save() {
    writeToFile();
}

bool DataManager::getLikeStatus(const QString& videoPath) const {
    QString relativePath = toRelativePath(videoPath);
    if (!videoData.contains(relativePath)) {
        return false; // 默认未点赞
    }
    return videoData[relativePath].toObject().value("liked").toBool(false);
}

void DataManager::setLikeStatus(const QString& videoPath, bool status) {
    QString relativePath = toRelativePath(videoPath);
    QJsonObject video = videoData[relativePath].toObject();
    video["liked"] = status;
    videoData[relativePath] = video;
    writeToFile();
}

bool DataManager::getCollectStatus(const QString& videoPath) const {
    QString relativePath = toRelativePath(videoPath);
    if (!videoData.contains(relativePath)) {
        return false;
    }
    return videoData[relativePath].toObject().value("collected").toBool(false);
}

void DataManager::setCollectStatus(const QString& videoPath, bool status) {
    QString relativePath = toRelativePath(videoPath);
    QJsonObject video = videoData[relativePath].toObject();
    video["collected"] = status;
    videoData[relativePath] = video;
    writeToFile();
}

QList<QVariantMap> DataManager::getComments(const QString& videoPath) const {
    QString relativePath = toRelativePath(videoPath);
    if (!videoData.contains(relativePath)) {
        return {};
    }
    QJsonArray comments = videoData[relativePath].toObject().value("comments").toArray();
    QList<QVariantMap> result;
    for (const QJsonValue& comment : comments) {
        result.append(comment.toObject().toVariantMap());
    }
    return result;
}

void DataManager::addComment(const QString& videoPath, const QString& username, const QString& content) {
    QString relativePath = toRelativePath(videoPath);
    QJsonObject video = videoData[relativePath].toObject();
    QJsonArray comments = video.value("comments").toArray();

    QJsonObject newComment;
    newComment["id"] = generateUniqueCommentId();
    newComment["username"] = username;
    newComment["content"] = content;

    comments.append(newComment);
    video["comments"] = comments;
    videoData[relativePath] = video;

    writeToFile();
}

void DataManager::editComment(const QString& videoPath, int commentId, const QString& username, const QString& content) {
    QString relativePath = toRelativePath(videoPath);
    QJsonObject video = videoData[relativePath].toObject();
    QJsonArray comments = video.value("comments").toArray();

    for (int i = 0; i < comments.size(); ++i) {
        QJsonObject comment = comments[i].toObject();
        if (comment["id"].toInt() == commentId) {
            comment["username"] = username;
            comment["content"] = content;
            comments[i] = comment;
            break;
        }
    }
    video["comments"] = comments;
    videoData[relativePath] = video;

    writeToFile();
}

void DataManager::deleteComment(const QString& videoPath, int commentId) {
    QString relativePath = toRelativePath(videoPath);
    QJsonObject video = videoData[relativePath].toObject();
    QJsonArray comments = video.value("comments").toArray();

    for (int i = 0; i < comments.size(); ++i) {
        if (comments[i].toObject()["id"].toInt() == commentId) {
            comments.removeAt(i);
            break;
        }
    }
    video["comments"] = comments;
    videoData[relativePath] = video;

    writeToFile();
}

QString DataManager::toRelativePath(const QString& absolutePath) const {
    QDir baseDir(baseFolderPath);
    return baseDir.relativeFilePath(absolutePath);
}

QString DataManager::toAbsolutePath(const QString& relativePath) const {
    QDir baseDir(baseFolderPath);
    return baseDir.absoluteFilePath(relativePath);
}

void DataManager::initializeUsedCommentIds() {
    usedCommentIds.clear();
    for (const QString& key : videoData.keys()) {
        QJsonArray comments = videoData[key].toObject().value("comments").toArray();
        for (const QJsonValue& comment : comments) {
            int id = comment.toObject().value("id").toInt();
            usedCommentIds.insert(id);
        }
    }
}

int DataManager::generateUniqueCommentId() {
    static int counter = 1;
    while (usedCommentIds.contains(counter)) {
        ++counter;
    }
    usedCommentIds.insert(counter);
    return counter;
}

void DataManager::writeToFile() {
    QFile file(jsonFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(videoData);
        file.write(doc.toJson());
        file.close();
    }
}

