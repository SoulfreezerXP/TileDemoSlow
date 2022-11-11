#include <QApplication>
#include <QFrame>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QOpenGLWidget>
#include <QStyleOption>

class View : public QFrame
{
    QGraphicsView * gView{ nullptr };
public:
    explicit View( QWidget *parent = nullptr ) : QFrame( parent ),
                                                 gView( new QGraphicsView( this ) )
    {
        gView->setViewport( new QOpenGLWidget( ) );
        gView->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
        gView->setRenderHint( QPainter::Antialiasing, false );
        gView->setDragMode( QGraphicsView::RubberBandDrag );
        gView->setOptimizationFlags( QGraphicsView::DontSavePainterState );
        gView->setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
        gView->setAlignment( Qt::AlignTop | Qt::AlignLeft );

        auto topLayout = new QGridLayout;
        topLayout->addWidget( gView, 1, 0 );
        setLayout( topLayout );
    }

    auto getView() const -> QGraphicsView& { return *gView; }
};

class Tile : public QGraphicsPixmapItem
{
public:
    explicit Tile( size_t x, size_t y, const QPixmap &graphic )
    {
        setZValue( 1 );
        setAcceptHoverEvents( true );
        setPixmap( graphic );
        setPos( QPointF( x * 32 , y * 32 ) );
    }
protected:
    auto paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) -> void override
    {
        QGraphicsPixmapItem::paint( painter, option, widget );

        if ( !( option->state & QStyle::State_MouseOver ) )
            return;

        auto oldBrush = painter->brush();
        painter->setBrush( QColor( 128, 128, 128 ) );
        painter->drawRect( 0, 0, 32, 32 );
        painter->setBrush( oldBrush );
    }
};

class MainWindow : public QWidget
{
    QGraphicsScene * scene{ nullptr };
public:
    explicit MainWindow( QWidget *parent = nullptr ) : QWidget( parent ),
                                                       scene( new QGraphicsScene( this ) )
    {
        auto pixmap = new QPixmap( "tile_empty.png" );
        auto view   = new View();
        auto layout = new QHBoxLayout;

        for ( size_t  y=0; y < 2000; y++ )
            for ( size_t  x=0; x < 2000; x++ )
                scene->addItem( new Tile( x, y, *pixmap ) );

        view->getView().setScene( scene );
        layout->addWidget( view );
        setLayout( layout );
        setWindowTitle( tr( "TileDemoSlow" ) );
    }
};

auto main( int argc, char *argv[] ) -> int
{
    QApplication app( argc, argv );
    app.setAttribute( Qt::AA_DontCreateNativeWidgetSiblings );
    MainWindow window;
    window.show();
    return app.exec();
}
