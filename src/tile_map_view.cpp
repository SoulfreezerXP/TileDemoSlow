#include "tile_map_view.h"
#include "qevent.h"

#include <QGraphicsView>
#include <QScrollBar>

namespace gamedev::soulcraft
{
    TileMapViewScrollBar::TileMapViewScrollBar( QWidget *parent ) : QScrollBar( parent )
    {}

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

