#pragma once

#include "tile_map.h"
#include <QWidget>
#include <QMainWindow>
#include <QTextEdit>
#include <QProgressBar>

class MainWindow : public QMainWindow
{
    TileMap *tileMapLeft;
    TileMap *tileMapRight;

    QTextEdit *textEdit;
    QProgressBar  *_progressbar;
    QString curFile;
public:
    explicit MainWindow( QWidget *parent = nullptr );
    auto keyPressEvent( QKeyEvent *event ) -> void override;

    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void loadFile(const QString &fileName);
private slots:
    //Onboard slots
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
};
