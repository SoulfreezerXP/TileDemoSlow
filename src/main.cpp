#include "main_window.h"

#include <QStyleFactory>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

auto main( int argc, char *argv[] ) -> int
{
    Q_INIT_RESOURCE( application );
#ifdef Q_OS_ANDROID
    QApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
#endif

    QApplication app(argc, argv);
    app.setAttribute( Qt::AA_DontCreateNativeWidgetSiblings );
    QCoreApplication::setOrganizationName( "Soulfreezer" );
    QCoreApplication::setApplicationName( "SoulCraft" );
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    qDebug() << QStyleFactory::keys();
    app.setStyle( QStyleFactory::create( "Fusion" ) ); //windowsvista", "Windows", "Fusion")

    QCommandLineParser parser;
    parser.setApplicationDescription( QCoreApplication::applicationName() );
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow mainWin;
    mainWin.showMaximized();
    return app.exec();

}
