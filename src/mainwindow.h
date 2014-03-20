#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "bookmark.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


protected:
    void closeEvent(QCloseEvent *event);


protected slots:
    void onAddStreamButtonClicked();
    void onRemoveStreamButtonClicked();
    void onWatchStreamButtonClicked();
    void onViewLogActionTriggered();
    void onAboutActionTriggered();
    void onAboutQtActionTriggered();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void loadSettings();
    void saveSettings();


private:
    void addBookmark(const Bookmark &bookmark);
    void removeBookmark(int index);
    void watchStream(const Bookmark &stream, const QString &player, const QString &quality);

    Ui::MainWindow *ui;
    QList<Bookmark> mBookmarks;
};

#endif // MAINWINDOW_H
