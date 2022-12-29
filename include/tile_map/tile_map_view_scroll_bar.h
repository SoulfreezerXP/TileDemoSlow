#pragma once

#include <QScrollBar>

namespace gamedev::soulcraft
{
    class TileMapViewScrollBar : public QScrollBar

    {
    Q_OBJECT;
    public:
        explicit TileMapViewScrollBar( QWidget *parent = nullptr );
    };
}
