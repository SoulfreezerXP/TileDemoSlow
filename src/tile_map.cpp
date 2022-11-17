#include "tile_map.h"

#include <QBoxLayout>
#include <QTimer>
#include <QKeyEvent>
#include <QOpenGLWidget>

namespace gamedev::soulcraft
{
    void TileMap::setViewport( const Vector2D& refVecViewportPositionInPixel,
                               const Vector2D& refVecViewportDimensionInPixel )
    {
        vecViewportPositionInPixel = refVecViewportPositionInPixel;
        vecViewportDimensionInPixel = refVecViewportDimensionInPixel;
    }

    void TileMap::createTiles( const Vector2D& refVecTileDimensionInPixel,
                               const Vector3D& refVecMapDimensionInTiles,
                               const Tile& fillTile,
                               size_t sztLayerIndex )
    {
        Tile fillTileFinal( fillTile );
        fillTileFinal.setGraphicId( "empty" );

        vecTileDimensionInPixel = refVecTileDimensionInPixel;
        vecMapDimensionInTiles  = refVecMapDimensionInTiles;

        tiles.assign( vecMapDimensionInTiles.y, std::vector< Tile >( vecMapDimensionInTiles.x, fillTileFinal ) );

        for ( size_t x = 0; x < vecMapDimensionInTiles.x; ++x )
            for ( size_t y = 0; y < vecMapDimensionInTiles.y; ++y )
                {
                    //nothing to do at the moment
                }
    }

    Tile& TileMap::accessTile( size_t sztTilePosX, size_t sztTilePosY )
    {
        if ( sztTilePosX >= 0 && sztTilePosX < tiles.size() &&
             sztTilePosY >= 0 && sztTilePosY < tiles[ sztTilePosX ].size() )
        {
            return tiles[ sztTilePosX ] [ sztTilePosY ];
        }

        throw std::exception( "accessTile(...): Out of bounds!" );
    }

    auto TileMap::prepareRenderTiles() -> void
    {
        if ( vecViewportDimensionInPixel.x < 64 || vecViewportDimensionInPixel.y < 64 )
            return;

        //Clear scene
        scene->clear();

        //RenderTiles scene
        renderTiles.clear();

        //Create PixmapAtlas
        pixmapAtlas.add( "empty", QPixmap( "gfx/tile_empty.png" ) );
        pixmapAtlas.add( "mouse_over", QPixmap( "gfx/tile_mouse_over.png" ) );

        //Create RenderTiles
        const size_t numOfTilesX = ( vecViewportDimensionInPixel.x / 32 ) + 3;
        const size_t numOfTilesY = ( vecViewportDimensionInPixel.y / 32 ) + 3;

        for ( size_t  y=0; y < numOfTilesY; y++ )
        {
            renderTiles.push_back( std::vector< RenderTile* >() );
            for ( size_t  x=0; x < numOfTilesX; x++ )
                renderTiles[ y ].emplace_back( new RenderTile( x*32, y*32, "", pixmapAtlas ) );
        }

        //Add RenderTiles to scene
        for ( size_t  y=0; y < numOfTilesY; y++ )
            for ( size_t  x=0; x < numOfTilesX; x++ )
                scene->addItem( renderTiles[ y ][ x ] );
    }

    TileMap::TileMap( QWidget *parent )
    {
        //Scene
        scene = new TileMapScene( parent );

        //View
        view = new TileMapView( parent );
        view->setViewport( new QOpenGLWidget( ) );
        view->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
        view->setRenderHint( QPainter::Antialiasing, false );
        view->setDragMode( QGraphicsView::RubberBandDrag );
        view->setOptimizationFlags( QGraphicsView::DontSavePainterState );
        view->setTransformationAnchor(QGraphicsView::NoAnchor);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setAlignment( Qt::AlignTop | Qt::AlignLeft );
        view->setScene( scene );

        //Layout
        auto layoutV  = new QVBoxLayout;
        layoutV->addWidget( view );

        QWidget* container = new QWidget;
        container->setLayout( layoutV );
        setWidget( container );
        setWindowTitle( tr( "TileDemoSlow" ) );

        //Connect
        connect( view, &TileMapView::viewPortSizeChanged,
                 this, &TileMap::viewPortSizeChanged );
    }

    void TileMap::viewPortSizeChanged( QRect viewSize )
    {
        //Update Viewport sizes
        setViewport( Vector2D{ 0, 0 },
                     Vector2D{ static_cast< size_t >( viewSize.width() ),
                               static_cast< size_t >( viewSize.height() ) } );

        //Prepare RenderTiles (recreate if necessary)
        prepareRenderTiles();

        //Update map only, when a tilemap exists! ToDo: Check if it is big enough!
        if ( !tiles.empty() )
            updateMap();
    }

    auto TileMap::getCamera() -> Vector2Df
    {
        Vector2Df cameraPosition = Vector2Df{ static_cast< double >( vecCameraPosition.x ),
                                              static_cast< double >( vecCameraPosition.y ) };
        return cameraPosition;
    }

    auto TileMap::setCamera( const Vector2Df& vecCameraPositionParam ) -> std::optional< Vector2Df >
    {
        bool autoCorrected = false;
        size_t sztMaxCamX = 0;
        size_t sztMaxCamY = 0;

        if ( ( vecMapDimensionInTiles.x * vecTileDimensionInPixel.x ) > vecViewportDimensionInPixel.x )
            sztMaxCamX = ( vecMapDimensionInTiles.x * vecTileDimensionInPixel.x ) - vecViewportDimensionInPixel.x;

        if ( ( vecMapDimensionInTiles.y * vecTileDimensionInPixel.y ) > vecViewportDimensionInPixel.y )
            sztMaxCamY = ( vecMapDimensionInTiles.y * vecTileDimensionInPixel.y ) - vecViewportDimensionInPixel.y;

        //Calculations are done in ints, because it is so much easier to handle them!
        auto cameraPositionRoundX = static_cast< long long >( std::round( vecCameraPositionParam.x ) );
        auto cameraPositionRoundY = static_cast< long long >( std::round( vecCameraPositionParam.y ) );

        //Mostly, the values are correct, so we directly set them and correct them only in the error cases
        vecCameraPosition.x = static_cast< size_t >( cameraPositionRoundX );
        vecCameraPosition.y = static_cast< size_t >( cameraPositionRoundY );

        Vector2Df vecCameraPositionCorrected = vecCameraPositionParam;

        //check for corrections
        if ( cameraPositionRoundX < 0 )
        {
            vecCameraPosition.x = 0;
            vecCameraPositionCorrected.x = 0.0;
            autoCorrected = true;
        }

        if ( cameraPositionRoundY < 0 )
        {
            vecCameraPosition.y = 0;
            vecCameraPositionCorrected.y = 0.0;
            autoCorrected = true;
        }

        if (  cameraPositionRoundX > static_cast< long long >( sztMaxCamX ) )
        {
            vecCameraPosition.x = sztMaxCamX;
            vecCameraPositionCorrected.x = static_cast< double >( sztMaxCamX );
            autoCorrected = true;
        }

        if (  cameraPositionRoundY > static_cast< long long >( sztMaxCamY ) )
        {
            vecCameraPosition.y = sztMaxCamY;
            vecCameraPositionCorrected.y = static_cast< double >( sztMaxCamY );
            autoCorrected = true;
        }

        updateMap(); //Updates the scene

        if ( !autoCorrected )
            return std::nullopt;

        return std::make_optional< Vector2Df >(
                    std::move( vecCameraPositionCorrected ) );
    }

    auto TileMap::updateMap() -> void
    {
        size_t startX = vecCameraPosition.x / vecTileDimensionInPixel.x;
        size_t endX = ( vecCameraPosition.x + vecViewportDimensionInPixel.x - 1 ) / vecTileDimensionInPixel.x;

        if ( startX >= vecMapDimensionInTiles.x )
            startX = vecMapDimensionInTiles.x - 1;

        if ( startX < 0 )
            startX = 0;

        if ( endX >= vecMapDimensionInTiles.x )
            endX = vecMapDimensionInTiles.x - 1;

        if ( endX < 0 )
            endX = 0;

        size_t finalStartX = startX;
        size_t finalEndX = endX + 1;

        size_t startY = vecCameraPosition.y / vecTileDimensionInPixel.y;
        size_t endY = ( vecCameraPosition.y + vecViewportDimensionInPixel.y - 1 ) / vecTileDimensionInPixel.y;

        if ( startY >= vecMapDimensionInTiles.y )
            startY = vecMapDimensionInTiles.y - 1;

        if (startY < 0)
            startY = 0;

        if ( endY >= vecMapDimensionInTiles.y )
            endY = vecMapDimensionInTiles.y - 1;

        if (endY < 0)
            endY = 0;

        size_t finalStartY = startY;
        size_t finalEndY = endY;

        for ( size_t y = finalStartY; y <= finalEndY; ++y )
        {
            for ( size_t x = finalStartX; x <= finalEndX; ++x )
            {
                QPointF originPosition;
                originPosition.setX( static_cast< qreal >( x * vecTileDimensionInPixel.x ) );
                originPosition.setY( static_cast< qreal >( y * vecTileDimensionInPixel.y ) );
                const auto positionX  = originPosition.x() - static_cast< qreal >( vecCameraPosition.x );
                const auto positionY  = originPosition.y() - static_cast< qreal >( vecCameraPosition.y );

                auto renderTilesX = x - finalStartX;
                auto renderTilesY = y - finalStartY;
                renderTiles[ renderTilesY ][ renderTilesX ]->setPos( positionX, positionY );
                renderTiles[ renderTilesY ][ renderTilesX ]->setGraphicId( accessTile( y, x ).getGraphicId() );
            }
        }
    }
}
