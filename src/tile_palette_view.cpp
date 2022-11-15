#include "tile_palette_view.h"

#include <QVBoxLayout>
#include <QHeaderView>

TilePaletteView::TilePaletteView( QObject *parent ) : myModel( nullptr ),
                                                      view( new QTableView() )
{
    view->setModel( &myModel );

    auto layoutV  = new QVBoxLayout;
    layoutV->addWidget( view );

    QWidget* container = new QWidget;
    container->setLayout( layoutV );
    setWidget( container );

    setWindowTitle( tr( "Palette" ) );
}


