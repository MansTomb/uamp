#include "GoogleAuthWrapper.h"

GoogleAuthWrapper::GoogleAuthWrapper() {
    google = new QOAuth2AuthorizationCodeFlow;
    google->setScope("email");
    auto json = QFile(":/googleAuth/googleAuth.apps.googleusercontent.com.json");

    QJsonParseError error;
    json.open(QIODevice::ReadOnly);
    const auto bytes = json.readAll();
    document = QJsonDocument(QJsonDocument::fromJson(bytes, &error));
    qDebug() << error.errorString();
    json.close();

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
    google->setReplyHandler(reply_handler);
}

void GoogleAuthWrapper::StartAuth() {
    google->grant();
    connect(google, &QOAuth2AuthorizationCodeFlow::granted, this, [=]{
      reply = google->get(QUrl("https://www.googleapis.com/plus/v1/people/me"));
      connect(reply, &QNetworkReply::finished, this, [=]{qDebug() << reply->readAll();});
    });
}

GoogleAuthWrapper::~GoogleAuthWrapper() {
//    delete google;
//    delete reply;
}
