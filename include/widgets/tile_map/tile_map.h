#pragma once

#include "pixmap_atlas/pixmap_atlas.h"

#include "widgets/tile_map/tile.h"
#include "widgets/tile_map/tile_render_element.h"
#include "widgets/tile_map/tile_map_scene.h"
#include "widgets/tile_map/tile_map_view.h"
#include "widgets/tile_map/tile_map_view_scroll_bar.h"

#include "qobjectdefs.h"

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

    class TileMap : public QWidget
    {
        Q_OBJECT
    public:
        explicit TileMap( QWidget *parent = nullptr );

        auto setPixmapAtlas( PixmapAtlas &pixmapAtlasParam ) -> void;

        void createTiles( const Vector2D& refVecTileDimensionInPixel,
                          const Vector3D& refVecMapDimensionInTiles,
                          const Tile& fillTile,	size_t sztLayerIndex );

        Tile& accessTile( size_t sztTilePosX, size_t sztTilePosY );

        auto updateMap() -> void;

        auto getCamera( ) -> Vector2Df;
        auto setCamera( const Vector2Df& vecCameraPositionParam ) -> std::optional< Vector2Df >;

    public slots:
        void viewScrollBarVerticalValueChanged( int );
        void viewScrollBarHorizontalValueChanged( int );

        void viewMouseMoved( QMouseEvent *event );
        void viewMousePressed( QMouseEvent *event );
    private:
        auto resizeMap() -> void;

        void resizeEvent( QResizeEvent* event ) override;

        void setViewport( const Vector2D& refVecViewportPositionInPixel,
                          const Vector2D& refVecViewportDimensionInPixel );

        auto prepareRenderTiles() -> void;

        PixmapAtlas *pixmapAtlas{ nullptr };
        TileMapScene * scene{ nullptr };
        TileMapView * view{ nullptr };
        TileMapViewScrollBar * viewScrollBarVertical{ nullptr };
        TileMapViewScrollBar * viewScrollBarHorizontal{ nullptr };

        std::vector< std::vector< TileRendeElement* > > renderTiles;
        std::vector< std::vector< TileRendeElement* > > markTiles;
        std::vector< std::vector< Tile > > tiles;

        Vector2D vecViewportPositionInPixel{ 0, 0 };
        Vector2D vecViewportDimensionInPixel{ 0, 0 };
        Vector2D vecTileDimensionInPixel{ 0, 0 };
        Vector3D vecMapDimensionInTiles{ 0, 0, 0 };
        Vector2D vecCameraPosition{ 0, 0 };
        Vector2D vecNumOfMaxVisibleRenderTiles{ 0, 0 };
    };
};
