#include "editor.h"

#include <QKeyEvent>
#include <QBoxLayout>
#include <QTimer>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QFile>
#include <QSettings>
#include <QCoreApplication>
#include <QApplication>
#include <QScreen>
#include <QDir>
#include <QSaveFile>
#include <QFileDialog>
#include <QTableView>

namespace gamedev::soulcraft
{
    Editor::Editor( QWidget *parent ) : QMainWindow( parent ),
                                        tileAtlas( new EditorTileAtlas( parent ) ),
                                        tileMap( new EditorTileMap( parent ) )
    {
        setWindowTitle( tr("SoulCraft ©2022 by Soulfreezer[*]") );

        addDockWidget( Qt::LeftDockWidgetArea,  tileAtlas );
        addDockWidget( Qt::RightDockWidgetArea, tileMap );

       //Create necessary stuff
        createActions();
        createStatusBar();
        readSettings();
    }

    void Editor::resizeEvent( QResizeEvent* event )
    {
       QMainWindow::resizeEvent( event );

       /*const QRect availableGeometry =  QGuiApplication::primaryScreen()->availableGeometry();

       QList< QDockWidget * > docks;
       docks.append( canvas );
       docks.append( palette );
       //docks.append( tileMapLeft );
       //docks.append( tileMapRight );
       //docks.append( tileLayer );

       const double windowWidth =  availableGeometry.width();

       QList< int > dockSizes;
       dockSizes.append( 0.5 * windowWidth );
       dockSizes.append( 0.5 * windowWidth );

       //dockSizes.append( 0.4 * windowWidth );
       //dockSizes.append( 0.4 * windowWidth );
       //dockSizes.append( 0.1 * windowWidth );

       resizeDocks( docks, dockSizes, Qt::Horizontal );*/
    }

    void Editor::paste()
    {
        //Create PixmapAtlas
        /*pixmapAtlas.add( "", QPixmap( "gfx/tile_empty.png" ) );
        pixmapAtlas.add( "mouse_over", QPixmap( "gfx/tile_mouse_over.png" ) );
        pixmapAtlas.add( "marked", QPixmap( "gfx/tile_marked.png" ) );
        tileMapLeft->setPixmapAtlas( pixmapAtlas );
        tileMapRight->setPixmapAtlas( pixmapAtlas );

        //Create Tiles for TileMap - Left
        {
            Vector2D vecTileDimensionInPixelLocal{ 32, 32 };
            Vector3D vecMapDimensionInTilesLocal{ 64, 64, 3 };
            const Tile fillTile{ 0, 0, "" };
            tileMapLeft->createTiles( vecTileDimensionInPixelLocal, vecMapDimensionInTilesLocal, fillTile, 0 );

            srand( time( NULL ) );
            for ( size_t  y=0; y < vecMapDimensionInTilesLocal.y; y++ )
                for ( size_t  x=0; x < vecMapDimensionInTilesLocal.x; x++ )
                {
                    const int r = ( rand() % 2 ) + 1;

                    if (r == 1 )
                         tileMapLeft->accessTile( x, y ).setGraphicId( "" );
                    else
                         tileMapLeft->accessTile( x, y ).setGraphicId( "mouse_over" );
                }

            tileMapLeft->updateMap();
        }

        //Create Tiles for TileMap - Right
        {
            Vector2D vecTileDimensionInPixelLocal{ 32, 32 };
            Vector3D vecMapDimensionInTilesLocal{ 64, 64, 3 };
            const Tile fillTile{ 0, 0, "" };
            tileMapRight->createTiles( vecTileDimensionInPixelLocal, vecMapDimensionInTilesLocal, fillTile, 0 );

            srand( time( NULL ) );
            for ( size_t  y=0; y < vecMapDimensionInTilesLocal.y; y++ )
                for ( size_t  x=0; x < vecMapDimensionInTilesLocal.x; x++ )
                {
                    const int r = ( rand() % 2 ) + 1;

                    if (r == 1 )
                         tileMapRight->accessTile( x, y ).setGraphicId( "" );
                    else
                         tileMapRight->accessTile( x, y ).setGraphicId( "mouse_over" );
                }

            tileMapRight->updateMap();
        }*/
    }

    auto Editor::keyPressEvent( QKeyEvent *event ) -> void
    {
        //int camSpeed  = 1;
        //int camSpeedX = 0;
        //int camSpeedY = 0;

        //if( event->key() == Qt::Key_D && !event->isAutoRepeat())
        //{
        //    camSpeedX = 1 * camSpeed;

        //    auto timer = new QTimer(nullptr);
        //    connect(timer, &QTimer::timeout, this, [ this, camSpeedX, camSpeedY ]
        //    {
        //        tileMapLeft->setCamera( Vector2Df{ tileMapLeft->getCamera().x + camSpeedX,
        //                                           tileMapLeft->getCamera().y + camSpeedY } );
        //    });

        //    timer->start();

        //    /*tileMapLeft->setCamera( Vector2Df{ tileMapLeft->getCamera().x + 1,
        //                                       tileMapLeft->getCamera().y + camSpeedY } );*/
        //}
    }

    void Editor::closeEvent( QCloseEvent *event )
    {
        if ( maybeSave() )
        {
          writeSettings();
          event->accept();
        }
        else
        {
          event->ignore();
        }
    }

    //=====================================
    // FUNCS
    //=====================================
    void Editor::createActions()
    {
        QMenu *fileMenu = menuBar()->addMenu(tr("&Project"));
        QToolBar *fileToolBar = addToolBar(tr("File"));
        const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/resources/QTNew.png"));
        QAction *newAct = new QAction(newIcon, tr("&New"), this);
        newAct->setShortcuts(QKeySequence::New);
        newAct->setStatusTip(tr("Create a new project"));
        connect(newAct, &QAction::triggered, this, &Editor::newFile);
        fileMenu->addAction(newAct);
        fileToolBar->addAction(newAct);

        const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/resources/QTOpen.png"));
        QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
        openAct->setShortcuts(QKeySequence::Open);
        openAct->setStatusTip(tr("Open an existing project"));
        connect(openAct, &QAction::triggered, this, &Editor::open);
        fileMenu->addAction(openAct);
        fileToolBar->addAction(openAct);

        const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/resources/QTSave"));
        QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
        saveAct->setShortcuts(QKeySequence::Save);
        saveAct->setStatusTip(tr("Save the project to disk"));
        connect(saveAct, &QAction::triggered, this, &Editor::save);
        fileMenu->addAction(saveAct);
        fileToolBar->addAction(saveAct);

        const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
        QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &Editor::saveAs);
        saveAsAct->setShortcuts(QKeySequence::SaveAs);
        saveAsAct->setStatusTip(tr("Save the project under a new name"));

        fileMenu->addSeparator();

        const QIcon exitIcon = QIcon::fromTheme("application-exit");
        QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
        exitAct->setShortcuts(QKeySequence::Quit);
        exitAct->setStatusTip(tr("Exit the application"));

        QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
        QToolBar *editToolBar = addToolBar(tr("Edit"));
    #ifndef QT_NO_CLIPBOARD
        const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/resources/QTCut.png"));
        QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);
        cutAct->setShortcuts(QKeySequence::Cut);
        cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                                "clipboard"));
        //connect(cutAct, &QAction::triggered, textEdit, &QTextEdit::cut);
        editMenu->addAction(cutAct);
        editToolBar->addAction(cutAct);

        const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/resources/QTCopy.png"));
        QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
        copyAct->setShortcuts(QKeySequence::Copy);
        copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                                 "clipboard"));
        //connect(copyAct, &QAction::triggered, textEdit, &QTextEdit::copy);
        editMenu->addAction(copyAct);
        editToolBar->addAction(copyAct);

        const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/resources/QTPaste.png"));
        QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
        pasteAct->setShortcuts(QKeySequence::Paste);
        pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                                  "selection"));
        connect(pasteAct, &QAction::triggered, this, &Editor::paste);
        editMenu->addAction(pasteAct);
        editToolBar->addAction(pasteAct);

        menuBar()->addSeparator();

    #endif // !QT_NO_CLIPBOARD

        QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
        QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &Editor::about);
        aboutAct->setStatusTip(tr("Show the application's About box"));
    #ifndef QT_NO_CLIPBOARD
        cutAct->setEnabled(false);
        copyAct->setEnabled(false);
        //connect(textEdit, &QTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
        //connect(textEdit, &QTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
    #endif // !QT_NO_CLIPBOARD
    }

    void Editor::createStatusBar()
    {
        statusBar()->showMessage(tr("Ready"));
    }

    void Editor::readSettings()
    {
        /*QSettings settings( QCoreApplication::organizationName(), QCoreApplication::applicationName() );
        const QByteArray geometry = settings.value( "geometry", QByteArray() ).toByteArray();
        if (geometry.isEmpty())
        {
            const QRect availableGeometry =  QGuiApplication::primaryScreen()->availableGeometry();
            resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
            move((availableGeometry.width() - width()) / 2,
                 (availableGeometry.height() - height()) / 2);
        } else
        {
            restoreGeometry(geometry);
        }*/
    }

    void Editor::writeSettings()
    {
        //QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
        //settings.setValue("geometry", saveGeometry());
    }

    bool Editor::maybeSave()
    {
        //if (!textEdit->document()->isModified())
            //return true;
        /*const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret) {
        case QMessageBox::Save:
            return save();
        case QMessageBox::Cancel:
            return false;
        default:
            break;
        }*/

        return true;
    }

    void Editor::loadFile(const QString &fileName)
    {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
            return;
        }

        QTextStream in(&file);
    #ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    #endif
        //textEdit->setPlainText(in.readAll());
        //textEdit->setHtml(in.readAll());
    #ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
    #endif

        setCurrentFile(fileName);
        statusBar()->showMessage(tr("File loaded"), 2000);
    }

    bool Editor::saveFile(const QString &fileName)
    {
        QString errorMessage;

        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
        QSaveFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            //out << textEdit->toPlainText();
            //out << textEdit->toHtml();

            if (!file.commit()) {
                errorMessage = tr("Cannot write file %1:\n%2.")
                               .arg(QDir::toNativeSeparators(fileName), file.errorString());
            }
        } else {
            errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
        QGuiApplication::restoreOverrideCursor();

        if (!errorMessage.isEmpty()) {
            QMessageBox::warning(this, tr("Application"), errorMessage);
            return false;
        }

        setCurrentFile(fileName);
        statusBar()->showMessage(tr("File saved"), 2000);
        return true;
    }

    void Editor::setCurrentFile(const QString &fileName)
    {
        curFile = fileName;
        //textEdit->document()->setModified(false);
        setWindowModified(false);

        QString shownName = curFile;
        if (curFile.isEmpty())
            shownName = "untitled.txt";
        setWindowFilePath(shownName);
    }

    QString Editor::strippedName(const QString &fullFileName)
    {
        return QFileInfo(fullFileName).fileName();
    }

    //=====================================
    // SLOTS
    //=====================================

    void Editor::newFile()
    {
      if ( maybeSave() )
      {
        //textEdit->clear();
        setCurrentFile(QString());
      }
    }

    void Editor::open()
    {
        if (maybeSave()) {
            QString fileName = QFileDialog::getOpenFileName(this,QString(),QString(),"HTML-Files (*.html)");
            if (!fileName.isEmpty())
                loadFile(fileName);
        }
    }

    bool Editor::save()
    {
        if (curFile.isEmpty()) {
            return saveAs();
        } else {
            return saveFile(curFile);
        }
    }

    bool Editor::saveAs()
    {
        QFileDialog dialog(this);

        dialog.setDefaultSuffix("html");
        dialog.setNameFilter("HTML-Files (*.html)");

        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        if (dialog.exec() != QDialog::Accepted)
            return false;
        return saveFile(dialog.selectedFiles().first());
    }

    void Editor::about()
    {
       QMessageBox::about(this, tr("About SoulCraft"),
                tr("<b>SoulCraft</b> is a multiplatform tile-map editor for creating 2d-games with the <b>SoulEngine</b>"));
    }

    void Editor::documentWasModified()
    {
        //setWindowModified(textEdit->document()->isModified());
        setWindowModified(  true );
    }
}


