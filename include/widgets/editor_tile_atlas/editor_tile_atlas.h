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
    public:
         EditorTileAtlas( QObject *parent = nullptr );
    };
}
