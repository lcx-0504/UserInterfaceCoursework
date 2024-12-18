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
        videoData = QJsonObject(); // Initialize null
    }
    initializeUsedCommentIds();
}

void DataManager::save() {
    writeToFile();
}

bool DataManager::getLikeStatus(const QString& videoPath) const {
    QString relativePath = toRelativePath(videoPath);
    if (!videoData.contains(relativePath)) {
        return false; //No like is clicked by default
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

