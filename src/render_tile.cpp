#include "render_tile.h"
#include <QPainter>

RenderTile::RenderTile( size_t x,
                        size_t y,
                        const std::string &tilePaletteGraphicIdParam,
                        PixmapAtlas &pixmapAtlasParam ) : tilePaletteGraphicId( tilePaletteGraphicIdParam ),
                                                          pixmapAtlas( &pixmapAtlasParam )
{
    this->x = x;
    this->y = y;
    //setZValue( 1 );
    setAcceptHoverEvents( true );
    setPixmap( pixmapAtlas->get( tilePaletteGraphicIdParam ) );
    setPos( QPointF( x, y ) );
    setShapeMode( ShapeMode::BoundingRectShape );
}

auto RenderTile::setGraphicId( const std::string &tilePaletteGraphicIdParam ) -> void
{
    if ( tilePaletteGraphicId == tilePaletteGraphicIdParam )
        return;

    tilePaletteGraphicId = tilePaletteGraphicIdParam;
    setPixmap( pixmapAtlas->get( tilePaletteGraphicId ) );
}

auto RenderTile::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) -> void
{
    QGraphicsPixmapItem::paint( painter, option, widget );

    if ( !( option->state & QStyle::State_MouseOver ) )
        return;

    auto oldBrush = painter->brush();
    painter->setBrush( QColor( 128, 128, 128 ) );
    painter->drawRect( 0, 0, 32, 32 );
    painter->setBrush( oldBrush );
}

auto RenderTile::mousePressEvent( QGraphicsSceneMouseEvent *event ) -> void
{
    setGraphicId( "mouse_over" );
    QGraphicsPixmapItem::mousePressEvent( event );
    //setPos( QPointF( x+20, y ) );
    update();
}

