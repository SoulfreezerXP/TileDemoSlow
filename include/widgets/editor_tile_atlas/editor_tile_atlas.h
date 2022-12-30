#pragma once

#include <QDockWidget>
#include <QStandardItemModel>
#include <QTreeView>

namespace gamedev::soulcraft
{
    class TileMap;
}

namespace gamedev::soulcraft
{
    class EditorTileAtlas : public QDockWidget
    {
        Q_OBJECT
        QStandardItemModel *sheetListModel;
        QTreeView *sheetList;
        TileMap *sheetTileMap;

        size_t newSheetsCounter{ 0 };

        auto findSheetListRootItem() -> QStandardItem *;
        auto findSheetListNextNewNumber() -> std::optional< size_t >;
    private slots:
         auto addSheet() -> void;
         auto deleteSheet() -> void;
         auto sheetChanged( QStandardItem *item ) -> void;
    public:
         EditorTileAtlas( QObject *parent = nullptr );
    };
}
