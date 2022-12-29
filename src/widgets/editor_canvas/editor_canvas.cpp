#include "widgets/editor_canvas/editor_canvas.h"

#include <QMenuBar>
#include <QToolBar>
#include <QVBoxLayout>

namespace gamedev::soulcraft
{
    EditorCanvas::EditorCanvas( QObject *parent ) : model( new QStandardItemModel() ),
                                                    view( new QTreeView() ),
                                                    tileMap( new TileMap() )
    {
        QStandardItem *rootNode = model->invisibleRootItem();

        //defining a couple of items
        QStandardItem *americaItem = new QStandardItem("America");
        QStandardItem *mexicoItem =  new QStandardItem("Canada");
        QStandardItem *usaItem =     new QStandardItem("USA");
        QStandardItem *bostonItem =  new QStandardItem("Boston");
        QStandardItem *europeItem =  new QStandardItem("Europe");
        QStandardItem *italyItem =   new QStandardItem("Italy");
        QStandardItem *romeItem =    new QStandardItem("Rome");
        QStandardItem *veronaItem =  new QStandardItem("Verona");

        //building up the hierarchy
        rootNode->    appendRow(americaItem);
        rootNode->    appendRow(europeItem);
        americaItem-> appendRow(mexicoItem);
        americaItem-> appendRow(usaItem);
        usaItem->     appendRow(bostonItem);
        europeItem->  appendRow(italyItem);
        italyItem->   appendRow(romeItem);
        italyItem->   appendRow(veronaItem);

        view->setModel( model );
        view->expandAll();

        auto layoutH  = new QHBoxLayout;
        layoutH->addWidget( view, 1 );
        layoutH->addWidget( tileMap, 5 );

        auto toolLayout = new QVBoxLayout();
        auto toolbar = new QToolBar;
        auto menuBar = new QMenuBar(this);

        toolLayout->addWidget( menuBar );
        toolLayout->addWidget( toolbar );
        toolLayout->addLayout( layoutH );

        QWidget* container = new QWidget;
        container->setLayout( toolLayout );
        setWidget( container );
        setWindowTitle( tr( "Canvas" ) );

        //----------------------------------------------------
        //Some large boilerplate for toolbar/menubar-demo
        //----------------------------------------------------
        QMenu *fileMenu = menuBar->addMenu(tr("&File"));
        const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/resources/new.png"));
        QAction *newAct = new QAction(newIcon, tr("&New"), this);
        newAct->setShortcuts(QKeySequence::New);
        newAct->setStatusTip(tr("Create a new file"));
        //connect(newAct, &QAction::triggered, this, &Editor::newFile);
        fileMenu->addAction(newAct);
        toolbar->addAction(newAct);

        const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/resources/open.png"));
        QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
        openAct->setShortcuts(QKeySequence::Open);
        openAct->setStatusTip(tr("Open an existing file"));
        //connect(openAct, &QAction::triggered, this, &Editor::open);
        fileMenu->addAction(openAct);
        toolbar->addAction(openAct);

        const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/resources/save.png"));
        QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
        saveAct->setShortcuts(QKeySequence::Save);
        saveAct->setStatusTip(tr("Save the document to disk"));
        //connect(saveAct, &QAction::triggered, this, &Editor::save);
        fileMenu->addAction(saveAct);
        toolbar->addAction(saveAct);

        const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
        QAction *saveAsAct = new QAction(saveAsIcon, tr("Save &As..."), this);
        saveAct->setShortcuts(QKeySequence::SaveAs);
        saveAct->setStatusTip(tr("Save the document under a new name"));
        //connect(saveAct, &QAction::triggered, this, &Editor::save);
        fileMenu->addAction(saveAsAct);

        fileMenu->addSeparator();

        const QIcon exitIcon = QIcon::fromTheme("application-exit");
        QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
        exitAct->setShortcuts(QKeySequence::Quit);
        exitAct->setStatusTip(tr("Exit the application"));

        toolbar->addSeparator();

        QMenu *editMenu = menuBar->addMenu(tr("&Edit"));
    #ifndef QT_NO_CLIPBOARD
        const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/resources/cut.png"));
        QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);
        cutAct->setShortcuts(QKeySequence::Cut);
        cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                                "clipboard"));
        //connect(cutAct, &QAction::triggered, textEdit, &QTextEdit::cut);
        editMenu->addAction(cutAct);
        toolbar->addAction(cutAct);

        const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/resources/copy.png"));
        QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
        copyAct->setShortcuts(QKeySequence::Copy);
        copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                                 "clipboard"));
        //connect(copyAct, &QAction::triggered, textEdit, &QTextEdit::copy);
        editMenu->addAction(copyAct);
        toolbar->addAction(copyAct);

        const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/resources/paste.png"));
        QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
        pasteAct->setShortcuts(QKeySequence::Paste);
        pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                                  "selection"));
        //connect(pasteAct, &QAction::triggered, this, &Editor::paste);
        editMenu->addAction(pasteAct);
        toolbar->addAction(pasteAct);

        menuBar->addSeparator();

    #endif // !QT_NO_CLIPBOARD

        QMenu *helpMenu = menuBar->addMenu(tr("&Help"));
        const QIcon aboutIcon = QIcon::fromTheme("");
        QAction *aboutAct = new QAction(aboutIcon, tr("&About"), this);
        aboutAct->setStatusTip(tr("Show the application's About box"));
        helpMenu->addAction(aboutAct);

    #ifndef QT_NO_CLIPBOARD
        cutAct->setEnabled(false);
        copyAct->setEnabled(false);
        //connect(textEdit, &QTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
        //connect(textEdit, &QTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
    #endif // !QT_NO_CLIPBOARD

        //----------------------------------------------------
        //Ende of  boilerplate for toolbar/menubar-demo
        //----------------------------------------------------
    }
}



