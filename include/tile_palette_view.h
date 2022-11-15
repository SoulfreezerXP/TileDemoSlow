#pragma once

#include "tile_palette_model.h"

#include <QDockWidget>
#include <QTableView>

class TilePaletteView : public QDockWidget
{
    Q_OBJECT
    TilePaletteModel myModel;
public:
     TilePaletteView( QObject *parent = nullptr );
     QTableView *view;
};
