#include "widgets/editor_canvas/editor_canvas.h"

#include <QVBoxLayout>
#include <QHeaderView>

namespace gamedev::soulcraft
{
    EditorCanvas::EditorCanvas( QObject *parent ) : model( new QStandardItemModel() ),
                                                    view( new QTreeView() ),
                                                    tileMap( new TileMap() )
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

        view->setModel( model );
        view->expandAll();

        auto layoutH  = new QHBoxLayout;
        layoutH->addWidget( view, 1 );
        layoutH->addWidget( tileMap, 5 );

        QWidget* container = new QWidget;
        container->setLayout( layoutH );
        setWidget( container );

        setWindowTitle( tr( "Canvas" ) );
    }
}



