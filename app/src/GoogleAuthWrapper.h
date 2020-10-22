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
  signals:
    void AuthComplete(QString username);
 private:
    void ParseProfile(const QByteArray& array);
    void Configure();

    QNetworkReply *reply {Q_NULLPTR};
    QOAuthHttpServerReplyHandler *reply_handler {Q_NULLPTR};
    QOAuth2AuthorizationCodeFlow *google {Q_NULLPTR};
    QJsonDocument document;
};
