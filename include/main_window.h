#pragma once

#include "render_tile.h"
#include "tile.h"
#include "view.h"

#include <QWidget>

#include <vector>

class MainWindow : public QWidget
{
    PixmapAtlas pixmapAtlas;
    QGraphicsScene * scene{ nullptr };
    View * viewA{ nullptr };
    View * viewB{ nullptr };
    View * viewC{ nullptr };
    View * viewD{ nullptr };

    std::vector< std::vector< RenderTile* > > renderTiles;

    QPoint _vecCameraPosition{ 0, 0 };
    QPoint _vecTileDimensionInPixel{ 0, 0 };
    QPoint _vecViewportDimensionInPixel{ 0 , 0 };
    QPoint _vecRenderTilesDimensionInTiles{ 0 , 0 };
    QPoint _vecMapDimensionInTiles{ 0 , 0 };
    std::vector< std::vector< Tile > > tileMap;
public:
    explicit MainWindow( QWidget *parent = nullptr );
    auto updateMap() -> void;
    auto keyPressEvent( QKeyEvent *event ) -> void override;
};
