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
    class EditorTileMap : public QDockWidget
    {
        Q_OBJECT
        QStandardItemModel *layerListModel;
        QTreeView *layerList;
        TileMap *layerTileMap;
    public:
         EditorTileMap( QObject *parent = nullptr );
    };
}
