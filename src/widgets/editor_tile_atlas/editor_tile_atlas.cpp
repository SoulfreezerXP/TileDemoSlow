#include "widgets/editor_tile_atlas/editor_tile_atlas.h"
#include "widgets/tile_map/tile_map.h"

#include <QMenuBar>
#include <QToolBar>
#include <QVBoxLayout>

namespace gamedev::soulcraft
{
    EditorTileAtlas::EditorTileAtlas( QObject *parent ) : sheetListModel( new QStandardItemModel() ),
                                                          sheetList( new QTreeView() ),
                                                          sheetTileMap( new TileMap() )
    {
        //Sheet-list
        QStandardItem *rootNode = sheetListModel->invisibleRootItem();
        QStandardItem *defaultItem = new QStandardItem( "Sheet1" );
        rootNode->appendRow( defaultItem );
        sheetList->setModel( sheetListModel );
        sheetList->expandAll();

        //Sheet-list menu
        auto sheetListMenuBar = new QMenuBar;
        auto sheetListToolBar = new QToolBar;

        {
            QMenu *sheetListMenu = sheetListMenuBar->addMenu( tr( "&Sheet" ) );
            const QIcon addIcon = QIcon::fromTheme( "document-add", QIcon( ":/resources/Add.png" ) );
            QAction *addAct = new QAction(addIcon, tr( "&Add new sheet" ), this );
            addAct->setShortcuts( QKeySequence::AddTab );
            addAct->setStatusTip(tr( "Adds a new sheet" ) );
            sheetListMenu->addAction( addAct );
            sheetListToolBar->addAction( addAct );

            const QIcon deleteIcon = QIcon::fromTheme( "document-delete", QIcon(":/resources/Delete.png"));
            QAction *deleteAct = new QAction(deleteIcon, tr( "&Delete sheet" ), this );
            deleteAct->setShortcuts( QKeySequence::Delete );
            deleteAct->setStatusTip( tr( "Deletes an existing sheet" ) );
            sheetListMenu->addAction( deleteAct );
            sheetListToolBar->addAction( deleteAct );
        }

        //Sheet-tile-map
        auto sheetTileMapMenuBar  = new QMenuBar;
        auto sheetTileMapToolBar  = new QToolBar;
        {
            QMenu *sheetTileMapMenu = sheetTileMapMenuBar->addMenu( tr( "&Tile" ) );
            const QIcon addIcon = QIcon::fromTheme( "document-add", QIcon( ":/resources/Add.png" ) );
            QAction *addAct = new QAction( addIcon, tr( "&Add new tile(s)" ), this );
            addAct->setShortcuts( QKeySequence::AddTab );
            addAct->setStatusTip(tr( "Adds new tile(s)" ) );
            sheetTileMapMenu->addAction( addAct );
            sheetTileMapToolBar->addAction( addAct );

            const QIcon deleteIcon = QIcon::fromTheme( "document-delete", QIcon(":/resources/Delete.png"));
            QAction *deleteAct = new QAction(deleteIcon, tr( "&Delete tiles(s)" ), this );
            deleteAct->setShortcuts( QKeySequence::Delete );
            deleteAct->setStatusTip( tr( "Deletes existing tiles(s)" ) );
            sheetTileMapMenu->addAction( deleteAct );
            sheetTileMapToolBar->addAction( deleteAct );
        }

        //Create the layout
        auto layout  = new QGridLayout;
        layout->addWidget( sheetListMenuBar, 0, 0 );
        layout->addWidget( sheetTileMapMenuBar, 0, 1 );
        layout->addWidget( sheetListToolBar, 1, 0 );
        layout->addWidget( sheetTileMapToolBar, 1, 1 );
        layout->addWidget( sheetList, 2, 0 );
        layout->addWidget( sheetTileMap, 2, 1 );

        layout->setColumnStretch( 0, 1 );
        layout->setColumnStretch( 1, 5 );

        QWidget* container = new QWidget;
        container->setLayout( layout );
        setWidget( container );
        setWindowTitle( tr( "Tile-Atlas" ) );
    }
}



