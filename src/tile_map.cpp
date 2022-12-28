#include "tile_map.h"

#include <QBoxLayout>
#include <QTimer>
#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QScrollBar>

namespace gamedev::soulcraft
{
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
        view->setAlignment( Qt::AlignTop | Qt::AlignLeft );
        view->setMouseTracking( true );
        view->setScene( scene );

        //View ScrollBars
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        viewScrollBarVertical = new TileMapViewScrollBar( parent );
        viewScrollBarVertical->setOrientation( Qt::Vertical );
        viewScrollBarVertical->setMinimum( 0 );
        viewScrollBarVertical->setSingleStep( 32 );
        viewScrollBarVertical->setMaximum( 0 );

        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        viewScrollBarHorizontal = new TileMapViewScrollBar( parent );
        viewScrollBarHorizontal->setOrientation( Qt::Horizontal );
        viewScrollBarHorizontal->setMinimum( 0 );
        viewScrollBarHorizontal->setSingleStep( 32 );
        viewScrollBarHorizontal->setMaximum( 0 );

        //Layout
        auto layoutH  = new QHBoxLayout;
        auto layoutV  = new QVBoxLayout;
        layoutV->addWidget( view );
        layoutV->addWidget( viewScrollBarHorizontal );
        layoutH->addLayout( layoutV );
        layoutH->addWidget( viewScrollBarVertical );
        QWidget* container = new QWidget;
        container->setLayout( layoutH );
        setWidget( container );
        setWindowTitle( tr( "TileDemoSlow" ) );

        connect( this, &QDockWidget::topLevelChanged,
                 this, &TileMap::topLevelChangedSlot );

        connect( viewScrollBarVertical, &TileMapViewScrollBar::valueChanged,
                 this, &TileMap::viewScrollBarVerticalValueChanged );

        connect( viewScrollBarHorizontal, &TileMapViewScrollBar::valueChanged,
                 this, &TileMap::viewScrollBarHorizontalValueChanged );

        connect( view, &TileMapView::mouseMoved,
                 this, &TileMap::viewMouseMoved );

        connect( view, &TileMapView::mousePressed,
                 this, &TileMap::viewMousePressed );
    }

    void TileMap::viewMouseMoved( QMouseEvent *event )
    {
        if( event->buttons() & Qt::LeftButton )
        {
            int x = ( event->pos().x() + getCamera().x ) / 32;
            int y = ( event->pos().y() + getCamera().y ) / 32;

            if ( x < 0 || x >= vecMapDimensionInTiles.x )
                return;

            if ( y < 0 || y >= vecMapDimensionInTiles.y )
                return;

            accessTile( x, y ).marked = true;
            updateMap();
        }
    }

    void TileMap::viewMousePressed( QMouseEvent *event )
    {
        if( event->buttons() & Qt::LeftButton )
        {
            int x = ( event->pos().x() + getCamera().x ) / 32;
            int y = ( event->pos().y() + getCamera().y ) / 32;

            if ( x < 0 || x >= vecMapDimensionInTiles.x )
                return;

            if ( y < 0 || y >= vecMapDimensionInTiles.y )
                return;

            accessTile( x, y ).marked = true;
            updateMap();
        }
    }


    void TileMap::viewScrollBarVerticalValueChanged( int value )
    {
        if ( getCamera().y != value )
            setCamera( Vector2Df{ getCamera().x, static_cast< double >( value ) } );
    }

    void TileMap::viewScrollBarHorizontalValueChanged( int value )
    {
        if ( getCamera().x != value )
            setCamera( Vector2Df{ static_cast< double >( value ), getCamera().y } );
    }

    auto TileMap::resizeMap() -> void
    {
        auto newScrollGeoV   = viewScrollBarVertical->geometry();
        auto newScrollGeoH  =  viewScrollBarHorizontal->geometry();
        newScrollGeoV.setHeight( view->contentsRect().height() );
        newScrollGeoH.setWidth( view->contentsRect().width() );
        viewScrollBarVertical->setGeometry( newScrollGeoV );
        viewScrollBarHorizontal->setGeometry( newScrollGeoH );

        //Update Viewport sizes
        setViewport( Vector2D{ 0, 0 },
                     Vector2D{ static_cast< size_t >( view->contentsRect().width() ),
                               static_cast< size_t >( view->contentsRect().height() ) } );

        //Prepare RenderTiles (recreate if necessary)
        prepareRenderTiles();

        //Update map only, when a tilemap and pixmapAtlas exists! ToDo: Check if it is big enough!
        if ( !tiles.empty() && pixmapAtlas )
            updateMap();
    }

    void TileMap::topLevelChangedSlot( bool topLevel )
    {
        resizeMap();
    }

    void TileMap::resizeEvent( QResizeEvent* event )
    {
        QDockWidget::resizeEvent( event );
        resizeMap();
    }

    auto TileMap::setPixmapAtlas( PixmapAtlas &pixmapAtlasParam ) -> void
    {
        pixmapAtlas = &pixmapAtlasParam;
    }

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
        fillTileFinal.setGraphicId( "" );

        vecTileDimensionInPixel = refVecTileDimensionInPixel;
        vecMapDimensionInTiles  = refVecMapDimensionInTiles;

        tiles.assign( vecMapDimensionInTiles.y, std::vector< Tile >( vecMapDimensionInTiles.x, fillTileFinal ) );

        for ( size_t y = 0; y < vecMapDimensionInTiles.y; ++y )
            for ( size_t x = 0; x < vecMapDimensionInTiles.x; ++x )
                {
                    //nothing to do at the moment
                }

        //Calculate Scrollbars
        {
            size_t sztMaxCamX = 0;
            size_t sztMaxCamY = 0;

            if ( ( vecMapDimensionInTiles.x * vecTileDimensionInPixel.x ) > vecViewportDimensionInPixel.x )
                sztMaxCamX = ( vecMapDimensionInTiles.x * vecTileDimensionInPixel.x ) - vecViewportDimensionInPixel.x;

            if ( ( vecMapDimensionInTiles.y * vecTileDimensionInPixel.y ) > vecViewportDimensionInPixel.y )
                sztMaxCamY = ( vecMapDimensionInTiles.y * vecTileDimensionInPixel.y ) - vecViewportDimensionInPixel.y;

            viewScrollBarVertical->setMinimum( 0 );
            viewScrollBarVertical->setMaximum( sztMaxCamY );

            viewScrollBarHorizontal->setMinimum( 0 );
            viewScrollBarHorizontal->setMaximum( sztMaxCamX );
        }
    }

    Tile& TileMap::accessTile( size_t sztTilePosX, size_t sztTilePosY )
    {
        if ( sztTilePosY >= 0 && sztTilePosY < tiles.size() &&
             sztTilePosX >= 0 && sztTilePosX < tiles[ sztTilePosY ].size() )

        {
            return tiles[ sztTilePosY ] [ sztTilePosX ];
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

        //MarkTiles
        markTiles.clear();

        //Create RenderTiles
        vecNumOfMaxVisibleRenderTiles.x = vecViewportDimensionInPixel.x % 32 == 0 ?
                    ( vecViewportDimensionInPixel.x / 32 ) + 1 : ( vecViewportDimensionInPixel.x / 32 ) + 2;

        vecNumOfMaxVisibleRenderTiles.y = vecViewportDimensionInPixel.y % 32 == 0 ?
                    ( vecViewportDimensionInPixel.y / 32 ) + 1 : ( vecViewportDimensionInPixel.y / 32 ) + 2;

        for ( size_t  y=0; y < vecNumOfMaxVisibleRenderTiles.y; y++ )
        {
            renderTiles.push_back( std::vector< RenderTile* >() );

            markTiles.push_back( std::vector< RenderTile* >() );

            for ( size_t  x=0; x < vecNumOfMaxVisibleRenderTiles.x; x++ )
            {
                QPixmap emptyPixmap;

                if ( pixmapAtlas )
                    emptyPixmap = pixmapAtlas->get( "" );

                renderTiles[ y ].emplace_back( new RenderTile( x*32, y*32, emptyPixmap ) );

                markTiles[ y ].emplace_back( new RenderTile( x*32, y*32, emptyPixmap ) );
            }
        }

        //Add RenderTiles to scene
        for ( size_t  y=0; y < vecNumOfMaxVisibleRenderTiles.y; y++ )
            for ( size_t  x=0; x < vecNumOfMaxVisibleRenderTiles.x; x++ )
            {
                scene->addItem( renderTiles[ y ][ x ] );
            }

        //Add markTiles to scene
        for ( size_t  y=0; y < vecNumOfMaxVisibleRenderTiles.y; y++ )
            for ( size_t  x=0; x < vecNumOfMaxVisibleRenderTiles.x; x++ )
            {
                scene->addItem( markTiles[ y ][ x ] );
            }

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

        //Update Scrollbars
        if ( viewScrollBarVertical->value() != vecCameraPosition.y )
            viewScrollBarVertical->setValue( vecCameraPosition.y );

        if ( viewScrollBarHorizontal->value() != vecCameraPosition.x )
            viewScrollBarHorizontal->setValue( vecCameraPosition.x );

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
        size_t finalEndX = endX;

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

        const bool maxNumOfVisibleRenderTilesUsedX = ( finalEndX - finalStartX ) + 1 == vecNumOfMaxVisibleRenderTiles.x;
        if ( !maxNumOfVisibleRenderTilesUsedX )
            finalEndX = finalEndX + 1;

        const bool maxNumOfVisibleRenderTilesUsedY = ( finalEndY - finalStartY ) + 1 == vecNumOfMaxVisibleRenderTiles.y;
        if ( !maxNumOfVisibleRenderTilesUsedY )
            finalEndY = finalEndY + 1;

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
                markTiles[ renderTilesY ][ renderTilesX ]->setPos( positionX, positionY );

                if ( y == finalEndY && !maxNumOfVisibleRenderTilesUsedY )
                    continue;

               if ( x == finalEndX && !maxNumOfVisibleRenderTilesUsedX )
                   break;

               renderTiles[ renderTilesY ][ renderTilesX ]->setPixmap(
                   pixmapAtlas->get( accessTile( x, y ).getGraphicId() ) );

               if ( accessTile( x, y ).marked )
               {
                   markTiles[ renderTilesY ][ renderTilesX ]->setPixmap( pixmapAtlas->get( "marked" ) );
                   markTiles[ renderTilesY ][ renderTilesX ]->setVisible( true );
               }
               else
                   markTiles[ renderTilesY ][ renderTilesX ]->setVisible( false );
            }
        }
    }
}
