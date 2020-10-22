#pragma once

#include <QWidget>
#include <QDesktopServices>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtNetworkAuth/QOAuth2AuthorizationCodeFlow>
#include <QtNetworkAuth/QOAuthHttpServerReplyHandler>
#include <QtNetwork/QNetworkReply>

class GoogleAuthWrapper : public QObject {
Q_OBJECT
 public :
    GoogleAuthWrapper();

    void StartAuth();
    ~GoogleAuthWrapper() override;
 private:
    void ParseProfile(const QByteArray& array);
    void Configure();

    QNetworkReply *reply;
    QOAuthHttpServerReplyHandler *reply_handler;
    QOAuth2AuthorizationCodeFlow *google;
    QJsonDocument document;
};
