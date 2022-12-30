#include "widgets/editor_tile_map/editor_tile_map.h"
#include "widgets/tile_map/tile_map.h"

#include <QMenuBar>
#include <QToolBar>
#include <QVBoxLayout>

namespace gamedev::soulcraft
{
    EditorTileMap::EditorTileMap( QObject *parent ) : layerListModel( new QStandardItemModel() ),
                                                      layerList( new QTreeView() ),
                                                      layerTileMap( new TileMap() )
    {
        //Layer-list
        QStandardItem *rootNode = layerListModel->invisibleRootItem();
        QStandardItem *defaultItem = new QStandardItem( "Layer1" );
        rootNode->appendRow( defaultItem );
        layerList->setModel( layerListModel );
        layerList->expandAll();

        //Layer-list menu
        auto layerListMenuBar = new QMenuBar;
        auto layerListToolBar = new QToolBar;

        {
            QMenu *layerListMenu = layerListMenuBar->addMenu( tr( "&Layer" ) );
            const QIcon addIcon = QIcon::fromTheme( "document-add", QIcon( ":/resources/Add.png" ) );
            QAction *addAct = new QAction(addIcon, tr( "&Add new layer" ), this );
            addAct->setShortcuts( QKeySequence::AddTab );
            addAct->setStatusTip(tr( "Adds a new layer" ) );
            layerListMenu->addAction( addAct );
            layerListToolBar->addAction( addAct );

            const QIcon deleteIcon = QIcon::fromTheme( "document-delete", QIcon(":/resources/Delete.png"));
            QAction *deleteAct = new QAction(deleteIcon, tr( "&Delete layer" ), this );
            deleteAct->setShortcuts( QKeySequence::Delete );
            deleteAct->setStatusTip( tr( "Deletes an existing layer" ) );
            layerListMenu->addAction( deleteAct );
            layerListToolBar->addAction( deleteAct );
        }

        //Layer-tile-map
        auto layerTileMapMenuBar  = new QMenuBar;
        auto layerTileMapToolBar  = new QToolBar;
        {
            QMenu *layerTileMapMenu = layerTileMapMenuBar->addMenu( tr( "&Tile" ) );
            const QIcon addIcon = QIcon::fromTheme( "document-add", QIcon( ":/resources/Add.png" ) );
            QAction *addAct = new QAction( addIcon, tr( "&Add new tile(s)" ), this );
            addAct->setShortcuts( QKeySequence::AddTab );
            addAct->setStatusTip(tr( "Adds new tile(s)" ) );
            layerTileMapMenu->addAction( addAct );
            layerTileMapToolBar->addAction( addAct );

            const QIcon deleteIcon = QIcon::fromTheme( "document-delete", QIcon(":/resources/Delete.png"));
            QAction *deleteAct = new QAction(deleteIcon, tr( "&Delete tiles(s)" ), this );
            deleteAct->setShortcuts( QKeySequence::Delete );
            deleteAct->setStatusTip( tr( "Deletes existing tiles(s)" ) );
            layerTileMapMenu->addAction( deleteAct );
            layerTileMapToolBar->addAction( deleteAct );
        }

        //Create the layout
        auto layout  = new QGridLayout;
        layout->addWidget( layerListMenuBar, 0, 0 );
        layout->addWidget( layerTileMapMenuBar, 0, 1 );
        layout->addWidget( layerListToolBar, 1, 0 );
        layout->addWidget( layerTileMapToolBar, 1, 1 );
        layout->addWidget( layerList, 2, 0 );
        layout->addWidget( layerTileMap, 2, 1 );

        layout->setColumnStretch( 0, 1 );
        layout->setColumnStretch( 1, 5 );

        QWidget* container = new QWidget;
        container->setLayout( layout );
        setWidget( container );
        setWindowTitle( tr( "Tile-Map" ) );
    }
}



