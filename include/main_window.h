#pragma once

#include "tile_map.h"
#include <QWidget>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    TileMap *tileMapLeft;
    TileMap *tileMapRight;
public:
    explicit MainWindow( QWidget *parent = nullptr );
    auto keyPressEvent( QKeyEvent *event ) -> void override;
};
