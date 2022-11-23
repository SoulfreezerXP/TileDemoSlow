#include "tile_map_view.h"

#include <QGraphicsView>
#include <QScrollBar>

namespace gamedev::soulcraft
{
    TileMapViewScrollBar::TileMapViewScrollBar( QWidget *parent ) : QScrollBar( parent )
    {}

    TileMapView::TileMapView( QWidget *parent ) : QGraphicsView( parent )
    {}
}

