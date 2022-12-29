#pragma once

#include <QGraphicsView>

namespace gamedev::soulcraft
{
    class TileMapScene : public QGraphicsScene
    {
    public:
        explicit TileMapScene( QWidget *parent = nullptr );
    };
}
