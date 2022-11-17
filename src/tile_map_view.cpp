#include "tile_map_view.h"

namespace gamedev::soulcraft
{
    TileMapView::TileMapView( QWidget *parent ) : QGraphicsView( parent )
    {}

    void TileMapView::resizeEvent( QResizeEvent* event )
    {
       QGraphicsView::resizeEvent( event );

       const QRect viewSize = contentsRect();

       qDebug() << "TileMapView::resizeEvent(): ViewportDimension is now:"
                << " width : " << viewSize.width()
                << " height: " << viewSize.height();

       emit viewPortSizeChanged( viewSize );
    }
}

