#include "main_window.h"

#include <QApplication>

auto main( int argc, char *argv[] ) -> int
{
    QApplication app( argc, argv );
    app.setAttribute( Qt::AA_DontCreateNativeWidgetSiblings );
    MainWindow window;
    window.show();
    return app.exec();
}
