#include "main_window.h"
#include "qevent.h"

#include <QBoxLayout>
#include <QTimer>

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ),
                                            tileMapLeft( new TileMap( parent ) ),
                                            tileMapRight( new TileMap( parent ) )
{
    setWindowTitle( tr( "TileDemo" ) );

    tileMapLeft->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tileMapRight->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    addDockWidget( Qt::LeftDockWidgetArea, tileMapLeft );
    addDockWidget( Qt::RightDockWidgetArea, tileMapRight );
}

auto MainWindow::keyPressEvent( QKeyEvent *event ) -> void
{
    int camSpeed  = 4;
    int camSpeedX = 0;
    int camSpeedY = 0;

    if( event->key() == Qt::Key_D )
    {
        camSpeedX = 1 *camSpeed;

        auto timer = new QTimer(nullptr);
        connect(timer, &QTimer::timeout, this, [ this, camSpeedX, camSpeedY ]
        {
            tileMapLeft->_vecCameraPosition.setX( tileMapLeft->_vecCameraPosition.x() + camSpeedX );
            tileMapLeft->_vecCameraPosition.setY( tileMapLeft->_vecCameraPosition.y() + camSpeedY );
            tileMapLeft->updateMap();
        });

        timer->start();
    }
}
