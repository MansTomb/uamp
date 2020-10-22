#include "GoogleAuthWrapper.h"

GoogleAuthWrapper::GoogleAuthWrapper() {
    google = new QOAuth2AuthorizationCodeFlow;
    google->setScope("profile");
    auto json = QFile(":/googleAuth/googleAuth.apps.googleusercontent.com.json");

    QJsonParseError error;
    json.open(QIODevice::ReadOnly);
    const auto bytes = json.readAll();
    document = QJsonDocument(QJsonDocument::fromJson(bytes, &error));
    qDebug() << error.errorString();
    json.close();

    google->setModifyParametersFunction([](QAbstractOAuth::Stage stage, QVariantMap* parameters) {
             if(stage == QAbstractOAuth::Stage::RequestingAccessToken){
                 QByteArray code = parameters->value("code").toByteArray();
                 (*parameters)["code"] = QUrl::fromPercentEncoding(code);
             }
           });

    connect(google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    Configure();
}

void GoogleAuthWrapper::Configure() {
    const auto object = document.object();
    const auto settingsObject = object["web"].toObject();
    const QUrl authUri(settingsObject["auth_uri"].toString());
    const auto clientId = settingsObject["client_id"].toString();
    const QUrl tokenUri(settingsObject["token_uri"].toString());
    const auto clientSecret(settingsObject["client_secret"].toString());
    const auto redirectUris = settingsObject["redirect_uris"].toArray();
    const QUrl redirectUri(redirectUris[0].toString()); // Get the first URI
    const auto port = static_cast<quint16>(redirectUri.port()); // Get the port

    google->setAuthorizationUrl(authUri);
    google->setClientIdentifier(clientId);
    google->setAccessTokenUrl(tokenUri);
    google->setClientIdentifierSharedKey(clientSecret);

    reply_handler = new QOAuthHttpServerReplyHandler(port, this);
    reply_handler->setCallbackPath("cb");
    google->setReplyHandler(reply_handler);
}

void GoogleAuthWrapper::StartAuth() {
    google->grant();
    connect(google, &QOAuth2AuthorizationCodeFlow::granted, this, [=]{
      reply = google->get(QUrl(QString("https://people.googleapis.com/v1/people/me?personFields=names&key=AIzaSyA3miH6kyGGzIsUU7ONSuFczKdqzxzkh9o"))); // https://www.googleapis.com/plus/v1/people/me
      connect(reply, &QNetworkReply::finished, this, [=]{ParseProfile(reply->readAll());});
    });
}

void GoogleAuthWrapper::ParseProfile(const QByteArray& array) {
    auto json = QJsonDocument(QJsonDocument::fromJson(array));
    auto mainObject = json.object();
    auto object = mainObject["names"].toArray();

    qDebug() << object[0].toObject()["displayName"].toString();
}

GoogleAuthWrapper::~GoogleAuthWrapper() {
//    delete google;
//    delete reply;
}
