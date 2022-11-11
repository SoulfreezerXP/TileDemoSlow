#include <QApplication>
#include <QFrame>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMetaEnum>
#include <QOpenGLWidget>
#include <QStyleOption>

#include <chrono>
#include <iostream>

template< typename EnumType >
QString ToString( const EnumType& enumValue )
{
    const char* enumName = qt_getEnumName( enumValue );
    const QMetaObject* metaObject = qt_getEnumMetaObject( enumValue );
    if (metaObject)
    {
        const int enumIndex = metaObject->indexOfEnumerator( enumName );
        return QString("%1::%2::%3").arg( metaObject->className(),
                                          enumName,
                                          metaObject->enumerator( enumIndex ).valueToKey( enumValue ) );
    }

    return QString("%1::%2").arg(enumName).arg(static_cast<int>(enumValue));
}

class Scene : public QGraphicsScene
{
public:
   explicit Scene( QWidget *parent = nullptr ) : QGraphicsScene( parent )
   {}

   bool event( QEvent *event ) override
   {
       auto start = std::chrono::high_resolution_clock::now();
        const bool ret = QGraphicsScene::event( event );
       auto stop = std::chrono::high_resolution_clock::now();
       auto duration = std::chrono::duration_cast< std::chrono::microseconds >( stop - start );
       std::cout << "EventType: " << ToString( event->type() ).toStdString() << " -> Duration: " << duration.count()
                 << " microseconds " << std::endl;
       return ret;
   }
};

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
        setShapeMode( ShapeMode::BoundingRectShape );
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
    Scene * scene{ nullptr };
    View * view{ nullptr };;
public:
    explicit MainWindow( QWidget *parent = nullptr ) : QWidget( parent ),
                                                       scene( new Scene( this ) ),
                                                       view( new View() )
    {
        auto pixmap = new QPixmap( "tile_empty.png" );
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
