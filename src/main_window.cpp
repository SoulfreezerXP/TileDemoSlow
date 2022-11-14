#include "main_window.h"
#include "qevent.h"

#include <QBoxLayout>
#include <QTimer>

MainWindow::MainWindow( QWidget *parent ) : QWidget( parent ),
                                            scene( new QGraphicsScene( this ) ),
                                            viewA( new View() ),
                                            viewB( new View() ),
                                            viewC( new View() ),
                                            viewD( new View() )
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
    _vecRenderTilesDimensionInTiles = { 22, 22 };

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

    viewA->getView().setScene( scene );
    viewB->getView().setScene( scene );
    viewC->getView().setScene( scene );
    viewD->getView().setScene( scene );

    //Layout
    auto layoutV  = new QVBoxLayout;
    auto layoutH1 = new QHBoxLayout;
    layoutH1->addWidget( viewA );
    layoutH1->addWidget( viewB );

    auto layoutH2 = new QHBoxLayout;
    layoutH2->addWidget( viewC ) ;
    layoutH2->addWidget( viewD );

    layoutV->addLayout( layoutH1 );
    layoutV->addLayout( layoutH2 );
    setLayout( layoutV );
    setWindowTitle( tr( "TileDemoSlow" ) );
}

auto MainWindow::updateMap() -> void
{
    QRect viewSize = viewA->getView().contentsRect();
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

    //qDebug() << "TileX: " << finalStartX << " TileY: " << finalEndX;
    //qDebug() << "_vecCameraPosition.x: " << _vecCameraPosition.x() << " _vecCameraPosition.y: " << _vecCameraPosition.y();
    //qDebug() << "_vecViewportDimensionInPixel.x: " << _vecViewportDimensionInPixel.x() << " _vecViewportDimensionInPixel.y: " << _vecViewportDimensionInPixel.y();
    //qDebug() << "------------------";

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

auto MainWindow::keyPressEvent( QKeyEvent *event ) -> void
{
    int camSpeed  = 4;
    int camSpeedX = 0;
    int camSpeedY = 0;

    if( event->key() == Qt::Key_A )
    {
        camSpeedX = -1 *camSpeed;

        auto timer = new QTimer(nullptr);
        connect(timer, &QTimer::timeout, this, [ this, camSpeedX, camSpeedY ]
        {
            _vecCameraPosition.setX( _vecCameraPosition.x() + camSpeedX );
            _vecCameraPosition.setY( _vecCameraPosition.y() + camSpeedY );
            updateMap();
        });

        timer->start();

    }

    if( event->key() == Qt::Key_D )
    {
        camSpeedX = 1 * camSpeed;

        auto timer = new QTimer( nullptr );
        connect(timer, &QTimer::timeout, this, [ this, camSpeedX, camSpeedY ]
        {
            _vecCameraPosition.setX( _vecCameraPosition.x() + camSpeedX );
            _vecCameraPosition.setY( _vecCameraPosition.y() + camSpeedY );
            updateMap();
        });

        timer->start();
    }

    /* if( event->key() == Qt::Key_W )
    {
        camSpeedY = -1 * camSpeed;
    }

    if( event->key() == Qt::Key_S )
    {
        camSpeedY = 1 * camSpeed;
    }*/

    //if (camSpeedX != 0 || camSpeedY != 0)
    //{
    //    _vecCameraPosition.setX( _vecCameraPosition.x() + camSpeedX );
    //    _vecCameraPosition.setY( _vecCameraPosition.y() + camSpeedY );
    //    updateMap();
    //}
}
