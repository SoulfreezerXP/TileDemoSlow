#pragma once

#include "render_tile.h"
#include "tile.h"
#include "view.h"

#include <QDockWidget>


#include <vector>

class TileMap : public QDockWidget
{
    PixmapAtlas pixmapAtlas;
    QGraphicsScene * scene{ nullptr };
    View * view{ nullptr };
    std::vector< std::vector< RenderTile* > > renderTiles;

    QPoint _vecTileDimensionInPixel{ 0, 0 };
    QPoint _vecViewportDimensionInPixel{ 0 , 0 };
    QPoint _vecRenderTilesDimensionInTiles{ 0 , 0 };
    QPoint _vecMapDimensionInTiles{ 0 , 0 };
    std::vector< std::vector< Tile > > tileMap;
public:
    QPoint _vecCameraPosition{ 0, 0 };
    explicit TileMap( QWidget *parent = nullptr );
    auto updateMap() -> void;
};
