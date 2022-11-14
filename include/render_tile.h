#pragma once

#include "pixmap_atlas.h"
#include <QStyleOption>
#include <QGraphicsPixmapItem>

class RenderTile : public QGraphicsPixmapItem
{
    size_t x{ 0 };
    size_t y{ 0 };
    std::string tilePaletteGraphicId;
    PixmapAtlas *pixmapAtlas{ nullptr };
public:
    explicit RenderTile( size_t x,
                         size_t y,
                         const std::string &tilePaletteGraphicIdParam,
                         PixmapAtlas &pixmapAtlasParam );

    auto setGraphicId( const std::string &tilePaletteGraphicIdParam ) -> void;
protected:
    auto paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) -> void override;
    auto mousePressEvent( QGraphicsSceneMouseEvent *event ) -> void override;
};
