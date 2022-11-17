#include "tile_map_view.h"

namespace gamedev::soulcraft
{
    TileMapView::TileMapView( QWidget *parent ) : QGraphicsView( parent )
    {}

    void TileMapView::resizeEvent( QResizeEvent* event )
    {
       QGraphicsView::resizeEvent( event );
       emit viewPortSizeChanged( contentsRect() );
    }
}

