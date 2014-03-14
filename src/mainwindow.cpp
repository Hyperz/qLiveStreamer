
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionAbout->setText("About " + QApplication::applicationName());

    // setup icons
    ui->btnAddStream->setIcon(QIcon::fromTheme("list-add", QIcon(":/images/add.png")));
    ui->btnRemoveStream->setIcon(QIcon::fromTheme("list-remove", QIcon(":/images/remove.png")));
    ui->btnWatchStream->setIcon(QIcon::fromTheme("media-playback-start", QIcon(":/images/play.png")));

    ui->actionAddStream->setIcon(ui->btnAddStream->icon());
    ui->actionRemoveStream->setIcon(ui->btnRemoveStream->icon());
    ui->actionExit->setIcon(QIcon::fromTheme("application-exit", QIcon(":/images/exit.png")));
    ui->actionViewLog->setIcon(QIcon(":/images/log.png"));
    ui->actionAbout->setIcon(this->windowIcon());
    ui->actionAboutQt->setIcon(QIcon(":/images/qt.png"));

    // connect signals & slots
    connect(ui->btnAddStream, SIGNAL(clicked()), this, SLOT(onAddStreamButtonClicked()));
    connect(ui->btnRemoveStream, SIGNAL(clicked()), this, SLOT(onRemoveStreamButtonClicked()));
    connect(ui->btnWatchStream, SIGNAL(clicked()), this, SLOT(onWatchStreamButtonClicked()));

    connect(ui->actionAddStream, SIGNAL(triggered()), this, SLOT(onAddStreamButtonClicked()));
    connect(ui->actionRemoveStream, SIGNAL(triggered()), this, SLOT(onRemoveStreamButtonClicked()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionViewLog, SIGNAL(triggered()), this, SLOT(onViewLogActionTriggered()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onAboutActionTriggered()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), this, SLOT(onAboutQtActionTriggered()));

    connect(ui->treeStreams, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(onWatchStreamButtonClicked()));


    setWindowTitle(QString("%1 v%2").arg(QApplication::applicationName(), QApplication::applicationVersion()));
    loadSettings();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}


void MainWindow::addBookmark(const Bookmark &bookmark)
{
    mBookmarks << bookmark;

    QStringList lst;
    lst << bookmark.serviceName() << bookmark.streamName();

    QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, lst);
    item->setToolTip(0, bookmark.url().toString());
    item->setToolTip(1, bookmark.url().toString());
    item->setIcon(0, bookmark.serviceIcon());

    ui->treeStreams->addTopLevelItem(item);
}


void MainWindow::removeBookmark(int index)
{
    mBookmarks.removeAt(index);
    delete ui->treeStreams->topLevelItem(index);
}


void MainWindow::onAddStreamButtonClicked()
{
    bool ok = false;
    QString txt = QInputDialog::getText(this, "Add Stream", "Stream URL (i.e. http://twitch.tv/some-stream):", QLineEdit::Normal, "http://", &ok);

    if (ok && !txt.isEmpty())
    {
        Bookmark newBookmark = Bookmark(QUrl(txt));

        if (newBookmark.service() == Bookmark::Unknown)
        {
            QMessageBox::critical(this, "Unknown Stream", "The stream URL you've entered is either invalid or not supported.");
            return;
        }

        // prevent dupes
        foreach (Bookmark bookmark, mBookmarks)
            if (bookmark == newBookmark)
                return;

        // add the bookmark
        addBookmark(newBookmark);
    }
}


void MainWindow::onRemoveStreamButtonClicked()
{
    QModelIndexList indexes = ui->treeStreams->selectionModel()->selectedRows();

    foreach (QModelIndex index, indexes)
    {
        if (index.row() < 0 || index.row() >= mBookmarks.length())
            continue;

        removeBookmark(index.row());
    }
}


void MainWindow::onWatchStreamButtonClicked()
{
    QModelIndexList indexes = ui->treeStreams->selectionModel()->selectedRows();

    QString player = (ui->chkUseCustomMediaPlayer->isChecked()) ? ui->txtCustomMediaPlayer->text() : ui->cbMediaPlayer->currentText();
    QString quality = ui->cbQuality->currentText();

    foreach (QModelIndex index, indexes)
    {
        if (index.row() < 0 || index.row() >= mBookmarks.length())
            continue;

        watchStream(mBookmarks[index.row()], player, quality);
    }
}


void MainWindow::onViewLogActionTriggered()
{
    // TODO: implement
}


void MainWindow::onAboutActionTriggered()
{
    QString format = "<h2>%1</h2>"
            "<b>Version:</b> %2<br/>"
            "<b>Author:</b> %3<br/>"
            "<b>Inspired by:</b> <a href=\"https://github.com/veridiam/glivestreamer\">gLiveStreamer</a>";

    QString txt = format.arg(QApplication::applicationName(), QApplication::applicationVersion(), QApplication::organizationName());

    QMessageBox::about(this, ui->actionAbout->text(), txt);
}


void MainWindow::onAboutQtActionTriggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);

    QProcess *process = (QProcess *)sender();

    if (exitCode != EXIT_SUCCESS)
    {
        QString processOutput = process->readAll();
        QMessageBox::critical(this, "Livestreamer error", "Livestreamer output:\n\n" +
                              processOutput);
    }

    process->deleteLater();
}


void MainWindow::loadSettings()
{
    QSettings settings;
    QString mediaPlayer = settings.value("MediaPlayer", "vlc").toString();
    QString quality = settings.value("Quality", "best").toString();
    bool useCustomMediaPlayer = settings.value("UseCustomMediaPlayer", false).toBool();
    QString customMediaPlayer = settings.value("CustomMediaPlayer", "/path/to/media-player --fullscreen").toString();
    QStringList bookmarks = settings.value("Bookmarks").toStringList();
    
    ui->cbMediaPlayer->setEnabled(!useCustomMediaPlayer);
    ui->cbMediaPlayer->setCurrentIndex(ui->cbMediaPlayer->findText(mediaPlayer));
    ui->cbQuality->setCurrentIndex(ui->cbQuality->findText(quality));
    ui->chkUseCustomMediaPlayer->setChecked(useCustomMediaPlayer);
    ui->txtCustomMediaPlayer->setEnabled(useCustomMediaPlayer);
    ui->txtCustomMediaPlayer->setText(customMediaPlayer);

    mBookmarks.clear();
    ui->treeStreams->clear();

    foreach (QString url, bookmarks)
        addBookmark(Bookmark(QUrl(url)));
}


void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("Version", QCoreApplication::applicationVersion());
    settings.setValue("MediaPlayer", ui->cbMediaPlayer->currentText());
    settings.setValue("Quality", ui->cbQuality->currentText());
    settings.setValue("UseCustomMediaPlayer", ui->chkUseCustomMediaPlayer->isChecked());
    settings.setValue("CustomMediaPlayer", ui->txtCustomMediaPlayer->text());

    QStringList bookmarks;
    foreach (Bookmark bookmark, mBookmarks)
        bookmarks << bookmark.url().toString();

    settings.setValue("Bookmarks", bookmarks);
}


void MainWindow::watchStream(const Bookmark &stream, const QString &player, const QString &quality)
{
    QStringList args;
    args << stream.url().toString() << quality << "--player" << player;

    QProcess *livestreamerProcess = new QProcess(this);
    connect(livestreamerProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onProcessFinished(int,QProcess::ExitStatus)));

    livestreamerProcess->start("livestreamer", args);
}

