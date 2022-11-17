#pragma once

#include <QGraphicsView>

namespace gamedev::soulcraft
{
    class TileMapView : public QGraphicsView
    {
        Q_OBJECT;
    public:
        explicit TileMapView( QWidget *parent = nullptr );
    signals:
        void viewPortSizeChanged( QRect viewSize );
    private:
        void resizeEvent( QResizeEvent* event ) override;
    };
}
