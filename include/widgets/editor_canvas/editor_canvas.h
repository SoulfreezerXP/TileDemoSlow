#pragma once

#include "widgets/tile_map/tile_map.h"

#include <QDockWidget>
#include <QTreeView>
#include <QStandardItemModel>

namespace gamedev::soulcraft
{
    class EditorCanvas : public QDockWidget
    {
        Q_OBJECT
        QStandardItemModel *model;
    public:
         EditorCanvas( QObject *parent = nullptr );
         QTreeView *view;
         TileMap *tileMap;
    };
}
