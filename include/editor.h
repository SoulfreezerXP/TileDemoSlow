#pragma once

#include "widgets/editor_canvas/editor_canvas.h"
#include "widgets/editor_palette/editor_palette.h"

#include <QWidget>
#include <QMainWindow>
#include <QTextEdit>
#include <QProgressBar>

namespace gamedev::soulcraft
{
    class Editor : public QMainWindow
    {
        PixmapAtlas pixmapAtlas;
        EditorCanvas *canvas;
        EditorPalette *palette;
        QTextEdit *textEdit;
        QProgressBar  *_progressbar;
        QString curFile;
    public:
        explicit Editor( QWidget *parent = nullptr );
        auto keyPressEvent( QKeyEvent *event ) -> void override;
        void resizeEvent( QResizeEvent* event ) override;

        void createActions();
        void createStatusBar();
        void readSettings();
        void writeSettings();
        bool maybeSave();
        bool saveFile(const QString &fileName);
        void setCurrentFile(const QString &fileName);
        QString strippedName(const QString &fullFileName);
        void loadFile(const QString &fileName);

        //Onboard slots
        void paste();
        void newFile();
        void open();
        bool save();
        bool saveAs();
        void about();
        void documentWasModified();

    protected:
        void closeEvent( QCloseEvent *event ) override;
    };
}
