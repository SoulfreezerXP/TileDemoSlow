#include "widgets/editor_tile_atlas/editor_tile_atlas.h"
#include "widgets/tile_map/tile_map.h"

#include <QMenuBar>
#include <QToolBar>
#include <QVBoxLayout>

#include <optional>
#include <set>

namespace gamedev::soulcraft
{
    EditorTileAtlas::EditorTileAtlas( QObject *parent ) : sheetListModel( new QStandardItemModel() ),
                                                          sheetList( new QTreeView() ),
                                                          sheetTileMap( new TileMap() )
    {
        //Sheet-list
        QStandardItem *rootNode = sheetListModel->invisibleRootItem();
        QStandardItem *defaultItem = new QStandardItem( "Sheets" );
        defaultItem->setEditable( false );
        rootNode->appendRow( defaultItem );
        sheetList->setModel( sheetListModel );
        sheetList->expandAll();
        connect( sheetListModel, &QStandardItemModel::itemChanged, this, &EditorTileAtlas::sheetChanged );

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
            connect( addAct, &QAction::triggered, this, &EditorTileAtlas::addSheet );

            const QIcon deleteIcon = QIcon::fromTheme( "document-delete", QIcon(":/resources/Delete.png"));
            QAction *deleteAct = new QAction(deleteIcon, tr( "&Delete sheet" ), this );
            deleteAct->setShortcuts( QKeySequence::Delete );
            deleteAct->setStatusTip( tr( "Deletes an existing sheet" ) );
            sheetListMenu->addAction( deleteAct );
            sheetListToolBar->addAction( deleteAct );
            connect( deleteAct, &QAction::triggered, this, &EditorTileAtlas::deleteSheet );
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

    auto EditorTileAtlas::findSheetListRootItem() -> QStandardItem *
    {
        auto foundItems = sheetListModel->findItems( "Sheets" );
        for ( const auto &foundItem : foundItems )
            if ( foundItem && foundItem->parent() == nullptr )
                return foundItem;

        return nullptr;
    }

    auto EditorTileAtlas::findSheetListNextNewNumber() -> std::optional< size_t >
    {
        constexpr size_t minNewNumber = 2;
        constexpr size_t maxNewNumber = 999;

        const auto foundNewSheetItems = sheetListModel->findItems( R"(^New Sheet$)",
                                                                   Qt::MatchRegularExpression |
                                                                   Qt::MatchRecursive );

        auto foundNewSheetItemsWithBraces = sheetListModel->findItems( R"(^New Sheet\(.+\)$)",
                                                                       Qt::MatchRegularExpression |
                                                                       Qt::MatchRecursive );
        if ( foundNewSheetItems.empty() )
            return std::nullopt;

        if ( foundNewSheetItemsWithBraces.empty() )
            return std::make_optional( minNewNumber );

        std::set< size_t > newNumbers;

        for ( const auto &foundItem : foundNewSheetItemsWithBraces )
        {
            const auto columnName      = foundItem->text();
            const auto indexBraceBegin = columnName.indexOf( "(" );
            const auto indexBraceEnd   = columnName.indexOf( ")" );
            const auto numberInBracet  = columnName.sliced( indexBraceBegin + 1,
                                                            indexBraceEnd - indexBraceBegin - 1 ).toInt();
            newNumbers.insert( numberInBracet ) ;
        }

        for ( size_t i = minNewNumber; i <= maxNewNumber; ++i )
            if ( !newNumbers.count( i )  )
                return std::make_optional( i );

        return std::nullopt;
    }

    auto EditorTileAtlas::sheetChanged( QStandardItem *item ) -> void
    {
        sheetList->sortByColumn( 0, Qt::SortOrder::AscendingOrder );
        findSheetListNextNewNumber();
    }

    auto EditorTileAtlas::deleteSheet() -> void
    {
        auto curIndex = sheetList->currentIndex();
        const auto rootItem = findSheetListRootItem();
        if ( rootItem  && rootItem->index() == curIndex )
            return;

        sheetListModel->removeRow( curIndex.row(), curIndex.parent() );
        sheetList->sortByColumn( 0, Qt::SortOrder::AscendingOrder );
    }

    auto EditorTileAtlas::addSheet() -> void
    {
        auto rootItem = findSheetListRootItem();

        if ( !rootItem )
            return;

        QString newSheetName( "New Sheet" );
        if ( findSheetListNextNewNumber().has_value() )
            newSheetName.append( QString( "(" ) + QString::number( *findSheetListNextNewNumber() ) + QString( ")" ) );

        rootItem->appendRow( new QStandardItem( newSheetName )  );
        sheetList->sortByColumn( 0, Qt::SortOrder::AscendingOrder );
    }
}



