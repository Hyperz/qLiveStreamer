
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

    if (mUrl.host().endsWith(QLatin1String("twitch.tv"), cs))       return Bookmark::Twitch;
    if (mUrl.host().endsWith(QLatin1String("justin.tv"), cs))       return Bookmark::Justin;
    if (mUrl.host().endsWith(QLatin1String("youtube.com"), cs))     return Bookmark::YouTube;
    if (mUrl.host().endsWith(QLatin1String("livestream.com"), cs))  return Bookmark::Livestream;
    if (mUrl.host().endsWith(QLatin1String("ustream.tv"), cs))      return Bookmark::UStream;
    if (mUrl.host().endsWith(QLatin1String("dailymotion.com"), cs)) return Bookmark::Dailymotion;

    return Bookmark::Unknown;
}


QString Bookmark::serviceName() const
{
    switch (service())
    {
    case Bookmark::Twitch:            return QLatin1String("Twitch.tv");
    case Bookmark::Justin:            return QLatin1String("Justin.tv");
    case Bookmark::YouTube:           return QLatin1String("YouTube.com");
    case Bookmark::Livestream:        return QLatin1String("Livestream.com");
    case Bookmark::UStream:           return QLatin1String("UStream.tv");
    case Bookmark::Dailymotion:       return QLatin1String("Dailymotion.com");
    case Bookmark::Unknown: default:  return QLatin1String("Unknown");
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
    if (mUrl.path().length() < 5) return QLatin1String("N/A");

    switch (service())
    {
    case Bookmark::Twitch:
    case Bookmark::Justin:
    case Bookmark::Livestream:
    case Bookmark::UStream:
    {
        QStringList parts = mUrl.path().split('/');
        return (parts.length() > 1) ? parts[1] : QLatin1String("N/A");
    }

    case Bookmark::YouTube:
        return mUrl.queryItemValue("v");

    case Bookmark::Dailymotion:
    {
        QStringList parts = mUrl.path().split('/');
        return (parts.length() > 2) ? parts[2] : QLatin1String("N/A");
    }

    default:
        return QLatin1String("N/A");
    }
}

