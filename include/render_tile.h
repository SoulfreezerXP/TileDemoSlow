#pragma once

#include <QStyleOption>
#include <QGraphicsPixmapItem>

namespace gamedev::soulcraft
{
    class RenderTile : public QGraphicsPixmapItem
    {
        size_t x{ 0 };
        size_t y{ 0 };
        qint64 pixMapCacheKey{ 0 };
    public:
        explicit RenderTile( size_t x,
                             size_t y,
                             const QPixmap &pixmapParam );

        auto setPixmap( const QPixmap &pixmapParam ) -> void;
    protected:
        auto paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) -> void override;
        auto mousePressEvent( QGraphicsSceneMouseEvent *event ) -> void override;
    };
}
