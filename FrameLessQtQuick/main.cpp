#include "MainWindow.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlProperty>
#include <QQuickWindow>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    // Qt::AA_UseSoftwareOpenGL, Qt::AA_UseDesktopOpenGL, Qt::AA_UseOpenGLES
    QGuiApplication::setAttribute(Qt::AA_UseOpenGLES);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Render options for Qt Quick
    QSurfaceFormat format;
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer); // Double Buffering on
    format.setSwapInterval(0);                            // v-sync off
    QSurfaceFormat::setDefaultFormat(format);

    QGuiApplication app(argc, argv);

    //! Native event filter should be installed on MainWindow class at this time.
    //! If you install NativeEventFilter later, it is really hard to make window to frameless window.
    MainWindow win_quick_window;
    app.installNativeEventFilter(&win_quick_window);

    // Set the Icon here.
    app.setWindowIcon(QIcon(":/icon/ApplicationIcon.png"));

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [&](QObject *obj, const QUrl &obj_url) {
            // Initialize window here
            if ((!obj && url == obj_url) || !win_quick_window.initWindow(engine))
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
