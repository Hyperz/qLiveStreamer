#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QUrlQuery>
#endif
#include "bookmark.h"


Bookmark::Bookmark(const QUrl &url) :
    mUrl(url)
{
}


QUrl Bookmark::url() const
{
    return mUrl;
}


void Bookmark::setUrl(const QUrl &url)
{
    mUrl = url;
}


Bookmark::Service Bookmark::service() const
{
    if (!mUrl.isValid()) return Bookmark::Unknown;

    Qt::CaseSensitivity cs = Qt::CaseInsensitive;

    if (mUrl.host().endsWith("twitch.tv", cs))       return Bookmark::Twitch;
    if (mUrl.host().endsWith("justin.tv", cs))       return Bookmark::Justin;
    if (mUrl.host().endsWith("youtube.com", cs))     return Bookmark::YouTube;
    if (mUrl.host().endsWith("livestream.com", cs))  return Bookmark::Livestream;
    if (mUrl.host().endsWith("ustream.tv", cs))      return Bookmark::UStream;
    if (mUrl.host().endsWith("dailymotion.com", cs)) return Bookmark::Dailymotion;

    return Bookmark::Unknown;
}


QString Bookmark::serviceName() const
{
    switch (service())
    {
    case Bookmark::Twitch:            return "Twitch.tv";
    case Bookmark::Justin:            return "Justin.tv";
    case Bookmark::YouTube:           return "YouTube.com";
    case Bookmark::Livestream:        return "Livestream.com";
    case Bookmark::UStream:           return "UStream.tv";
    case Bookmark::Dailymotion:       return "Dailymotion.com";
    case Bookmark::Unknown: default:  return "Unknown";
    }
}


QIcon Bookmark::serviceIcon() const
{
    switch (service())
    {
    case Bookmark::Twitch:            return QIcon(":/images/services/twitch.png");
    case Bookmark::Justin:            return QIcon(":/images/services/justin.png");
    case Bookmark::YouTube:           return QIcon(":/images/services/youtube.png");
    case Bookmark::Livestream:        return QIcon(":/images/services/livestream.png");
    case Bookmark::UStream:           return QIcon(":/images/services/ustream.png");
    case Bookmark::Dailymotion:       return QIcon(":/images/services/dailymotion.png");
    case Bookmark::Unknown: default:  return QIcon(":/images/services/unknown.png");
    }
}


QString Bookmark::streamName() const
{
    if (mUrl.path().length() < 5) return "N/A";

    switch (service())
    {
    case Bookmark::Twitch:
    case Bookmark::Justin:
    case Bookmark::Livestream:
    case Bookmark::UStream:
    {
        QStringList parts = mUrl.path().split('/');
        return (parts.length() > 1) ? parts[1] : "N/A";
    }

    case Bookmark::YouTube: {
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
        QUrlQuery q(mUrl.query());
        return q.queryItemValue("v");
#else
        return mUrl.queryItemValue("v");
#endif
    }

    case Bookmark::Dailymotion:
    {
        QStringList parts = mUrl.path().split('/');
        return (parts.length() > 2) ? parts[2] : "N/A";
    }

    default:
        return "N/A";
    }
}

