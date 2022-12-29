#include "widgets/tile_map/tile_map_view.h"
#include "qevent.h"

#include <QGraphicsView>
#include <QScrollBar>

namespace gamedev::soulcraft
{
    TileMapView::TileMapView( QWidget *parent ) : QGraphicsView( parent )
    {}

    void TileMapView::mouseMoveEvent(QMouseEvent *event)
    {
         emit mouseMoved( event );
    }

    void TileMapView::mousePressEvent(QMouseEvent *event)
    {
        emit mousePressed( event );
    }
}

