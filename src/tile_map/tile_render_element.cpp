#include "tile_map/tile_render_element.h"

#include <QMessageBox>
#include <QPainter>
#include <QPixmap>

namespace gamedev::soulcraft
{
    TileRendeElement::TileRendeElement( size_t x,
                                        size_t y,
                                        const QPixmap &pixmapParam )
    {
        this->x = x;
        this->y = y;
        setAcceptHoverEvents( false );                  //ATTENTION
        setAcceptedMouseButtons( Qt::NoButton );        //ATTENTION
        setPos( QPointF( x, y ) );
        setShapeMode( ShapeMode::BoundingRectShape );
        setPixmap( pixmapParam );
    }

    auto TileRendeElement::setPixmap( const QPixmap &pixmapParam ) -> void
    {
        if ( pixMapCacheKey != pixmapParam.cacheKey() )
        {
            QGraphicsPixmapItem::setPixmap( pixmapParam );
            pixMapCacheKey = pixmapParam.cacheKey();
        }
    }

    auto TileRendeElement::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) -> void
    {
        QGraphicsPixmapItem::paint( painter, option, widget );

        if ( !( option->state & QStyle::State_MouseOver ) )
            return;

        auto oldBrush = painter->brush();
        painter->setBrush( QColor( 128, 128, 128 ) );
        painter->drawRect( 0, 0, 32, 32 );
        painter->setBrush( oldBrush );
    }

    auto TileRendeElement::mousePressEvent( QGraphicsSceneMouseEvent *event ) -> void
    {
        QGraphicsPixmapItem::mousePressEvent( event );
        QMessageBox msgBox;
        msgBox.setText("Clicked");
        msgBox.exec();
    }
}

