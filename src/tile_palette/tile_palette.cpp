#include "tile_palette/tile_palette.h"

#include <QVBoxLayout>
#include <QHeaderView>

namespace gamedev::soulcraft
{
    TilePalette::TilePalette( QObject *parent ) : model( new QStandardItemModel() ),
                                                  view( new QTreeView() )
    {
        QStandardItem *rootNode = model->invisibleRootItem();

            //defining a couple of items
            QStandardItem *americaItem = new QStandardItem("America");
            QStandardItem *mexicoItem =  new QStandardItem("Canada");
            QStandardItem *usaItem =     new QStandardItem("USA");
            QStandardItem *bostonItem =  new QStandardItem("Boston");
            QStandardItem *europeItem =  new QStandardItem("Europe");
            QStandardItem *italyItem =   new QStandardItem("Italy");
            QStandardItem *romeItem =    new QStandardItem("Rome");
            QStandardItem *veronaItem =  new QStandardItem("Verona");

            //building up the hierarchy
            rootNode->    appendRow(americaItem);
            rootNode->    appendRow(europeItem);
            americaItem-> appendRow(mexicoItem);
            americaItem-> appendRow(usaItem);
            usaItem->     appendRow(bostonItem);
            europeItem->  appendRow(italyItem);
            italyItem->   appendRow(romeItem);
            italyItem->   appendRow(veronaItem);

        view->setModel(model);
        view->expandAll();

        auto layoutV  = new QVBoxLayout;
        layoutV->addWidget( view );

        QWidget* container = new QWidget;
        container->setLayout( layoutV );
        setWidget( container );

        setWindowTitle( tr( "Palette" ) );
    }
}



