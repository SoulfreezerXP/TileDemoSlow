#include "view.h"

#include <QGridLayout>
#include <QOpenGLWidget>

View::View( QWidget *parent ) : QFrame( parent ),
                                gView( new QGraphicsView( this ) )
{
    gView->setViewport( new QOpenGLWidget( ) );
    //gView->setRenderHint(QPainter::Antialiasing);

    gView->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    gView->setRenderHint( QPainter::Antialiasing, false );
    gView->setDragMode( QGraphicsView::RubberBandDrag );
    gView->setOptimizationFlags( QGraphicsView::DontSavePainterState );

    //gView->setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
    gView->setTransformationAnchor(QGraphicsView::NoAnchor);
    gView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gView->setAlignment( Qt::AlignTop | Qt::AlignLeft );

    auto topLayout = new QGridLayout;
    topLayout->addWidget( gView, 1, 0 );
    setLayout( topLayout );

}

auto View::getView() const -> QGraphicsView&
{
    return *gView;
}

