#pragma once

#include "tile.h"
#include "tile_map_scene.h"
#include "tile_map_view.h"
#include "render_tile.h"

#include <QDockWidget>
#include <vector>
#include <QPoint>
#include <QVector3D>

#include <vector>

namespace gamedev::soulcraft
{
    struct Vector2D { size_t x, y, z; };
    struct Vector2Df { double  x, y, z; };
    struct Vector3D { size_t x, y, z; };
    struct Vector3Df { double x, y, z; };

    class TileMap : public QDockWidget
    {
    public:
        explicit TileMap( QWidget *parent = nullptr );

        auto prepareRenderTiles() -> void;

        auto buildAndShow() -> void;

        void createTiles( const Vector2D& refVecTileDimensionInPixel,
                          const Vector3D& refVecMapDimensionInTiles,
                          const Tile& fillTile,	size_t sztLayerIndex );

        Tile& accessTile( size_t sztTilePosX, size_t sztTilePosY );

        auto getCamera( ) -> Vector2Df;
        auto setCamera( const Vector2Df& vecCameraPositionParam ) -> std::optional< Vector2Df >;
    public slots:
        void viewPortSizeChanged( QRect viewSize );
    private:
        void setViewport( const Vector2D& refVecViewportPositionInPixel,
                          const Vector2D& refVecViewportDimensionInPixel );
        auto updateMap() -> void;

        PixmapAtlas pixmapAtlas;
        TileMapScene * scene{ nullptr };
        TileMapView * view{ nullptr };
        std::vector< std::vector< RenderTile* > > renderTiles;
        std::vector< std::vector< Tile > > tiles;

        Vector2D vecViewportPositionInPixel{ 0, 0 };
        Vector2D vecViewportDimensionInPixel{ 0, 0 };
        Vector2D vecTileDimensionInPixel{ 0, 0 };
        Vector3D vecMapDimensionInTiles{ 0, 0, 0 };
        Vector2D vecCameraPosition{ 0, 0 };
    };
};
