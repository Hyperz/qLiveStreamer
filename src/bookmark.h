#ifndef STREAMBOOKMARK_H
#define STREAMBOOKMARK_H

#include <QtCore>
#include <QIcon>

class Bookmark
{
public:
    enum Service
    {
        Unknown = 0,
        Twitch,
        Justin,
        YouTube,
        Livestream,
        UStream,
        Dailymotion
    };

    Bookmark(const QUrl &url);

    QUrl url() const;
    void setUrl(const QUrl &url);
    Bookmark::Service service() const;
    QString serviceName() const;
    QIcon serviceIcon() const;
    QString streamName() const;


    inline bool operator ==(const Bookmark &other)
    {
        return (url() == other.url());
    }


private:
    QUrl mUrl;
};

#endif // STREAMBOOKMARK_H
