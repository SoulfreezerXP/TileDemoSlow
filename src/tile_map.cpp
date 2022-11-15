#include "tile_map.h"

#include <QBoxLayout>
#include <QTimer>
#include <QKeyEvent>

TileMap::TileMap( QWidget *parent ) : scene( new QGraphicsScene( parent ) ),
                                      view( new View() )
{
    //Set the basic params
    _vecCameraPosition = { 0, 0 };
    _vecTileDimensionInPixel = { 32, 32 };
    _vecViewportDimensionInPixel = { 0 , 0 }; //Determine later
    _vecMapDimensionInTiles = { 5000 , 5000 };

    //pixmapAtlas
    pixmapAtlas.add( "empty", QPixmap( "gfx/tile_empty.png" ) );
    pixmapAtlas.add( "mouse_over", QPixmap( "gfx/tile_mouse_over.png" ) );

    //TileMap
    srand( time( NULL ) );

    for ( size_t  y=0; y < _vecMapDimensionInTiles.y(); y++ )
    {
        tileMap.push_back( std::vector< Tile >() );

        for ( size_t  x=0; x < _vecMapDimensionInTiles.x(); x++ )
        {
            int r = ( rand() % 2 ) + 1;

            if (r == 1 )
                tileMap[ y ].emplace_back( x, y, "empty" );
            else
                tileMap[ y ].emplace_back( x, y, "mouse_over" );
        }
    }

    //RenderTiles
    _vecRenderTilesDimensionInTiles = { 45, 45 };

    for ( size_t  y=0; y < _vecRenderTilesDimensionInTiles.y(); y++ )
    {
        renderTiles.push_back( std::vector< RenderTile* >() );

        for ( size_t  x=0; x < _vecRenderTilesDimensionInTiles.x(); x++ )
        {
            auto id = tileMap[ y ][ x ].getGraphicId();
            renderTiles[ y ].emplace_back( new RenderTile( x*32, y*32, id, pixmapAtlas ) );
        }
    }

    //Add RenderTiles to scene
    for ( size_t  y=0; y < _vecRenderTilesDimensionInTiles.y(); y++ )
        for ( size_t  x=0; x < _vecRenderTilesDimensionInTiles.x(); x++ )
            scene->addItem( renderTiles[ y ][ x ] );

    view->getView().setScene( scene );

    //Layout
    auto layoutV  = new QVBoxLayout;
    layoutV->addWidget( view );

    QWidget* container = new QWidget;
    container->setLayout( layoutV );
    setWidget(container);
    setWindowTitle( tr( "TileDemoSlowB" ) );
}

auto TileMap::updateMap() -> void
{
    QRect viewSize = view->getView().contentsRect();
    _vecViewportDimensionInPixel = { viewSize.width(), viewSize.height() };

    size_t startX = _vecCameraPosition.x() / _vecTileDimensionInPixel.x();
    size_t endX = ( _vecCameraPosition.x() + _vecViewportDimensionInPixel.x() - 1 ) / _vecTileDimensionInPixel.x();

    if ( startX >= _vecMapDimensionInTiles.x() )
        startX = _vecMapDimensionInTiles.x() - 1;

    if ( startX < 0 )
        startX = 0;

    if ( endX >= _vecMapDimensionInTiles.x() )
        endX = _vecMapDimensionInTiles.x() - 1;

    if ( endX < 0 )
        endX = 0;

    size_t finalStartX = startX;
    size_t finalEndX = endX + 1;

    size_t startY = _vecCameraPosition.y() / _vecTileDimensionInPixel.y();
    size_t endY = ( _vecCameraPosition.y() + _vecViewportDimensionInPixel.y() - 1 ) / _vecTileDimensionInPixel.y();

    if ( startY >= _vecMapDimensionInTiles.y() )
        startY = _vecMapDimensionInTiles.y() - 1;

    if (startY < 0)
        startY = 0;

    if ( endY >= _vecMapDimensionInTiles.y() )
        endY = _vecMapDimensionInTiles.y() - 1;

    if (endY < 0)
        endY = 0;

    size_t finalStartY = startY;
    size_t finalEndY = endY;

    for ( size_t y = finalStartY; y <= finalEndY; ++y )
    {
        for ( size_t x = finalStartX; x <= finalEndX; ++x )
        {
            QPointF originPosition;
            originPosition.setX( static_cast< qreal >( x * _vecTileDimensionInPixel.x() ) );
            originPosition.setY( static_cast< qreal >( y * _vecTileDimensionInPixel.y() ) );
            const auto positionX  = originPosition.x() - static_cast< qreal >( _vecCameraPosition.x() );
            const auto positionY  = originPosition.y() - static_cast< qreal >( _vecCameraPosition.y() );

            auto renderTilesX = x - finalStartX;
            auto renderTilesY = y - finalStartY;
            renderTiles[ renderTilesY ][ renderTilesX ]->setPos( positionX, positionY );
            renderTiles[ renderTilesY ][ renderTilesX ]->setGraphicId( tileMap[ y ][ x ].getGraphicId() );
        }
    }
}
