#pragma once

#include <QScrollBar>
#include <QGraphicsView>

namespace gamedev::soulcraft
{
    class TileMapViewScrollBar : public QScrollBar

    {
        Q_OBJECT;
    public:
        explicit TileMapViewScrollBar( QWidget *parent = nullptr );
    };

    class TileMapView : public QGraphicsView
    {
        Q_OBJECT;
    public:
        explicit TileMapView( QWidget *parent = nullptr );
    };
}
