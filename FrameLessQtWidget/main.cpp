#include "MainWindow.hpp"
#include <QApplication>
#include <QMenuBar>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);

    MainWindow w;

    w.setResizeBorderWidth(6); //! You can set the resize border width.
    w.setTitlebarHeight(35);   //! You also can set the title bar height.

    // Customize menu bar
    QMenuBar *menubar = new QMenuBar(&w);
    menubar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    menubar->setStyleSheet(R"(
       QMenuBar {
           border-image: url(:/icon/Transparent.png);
           background-color: rgba(255, 255, 255, 0%);
           background-repeat: no-repeat;
       }
       QMenuBar::item {
           color: rgb(152, 160, 175);
       }
       QMenuBar::item[active="false"] {
           color: rgb(112, 120, 135);
       }
       QMenuBar::item:selected {
           spacing: 3px;
           padding: 2px 10px;
           background-color: rgb(52, 56, 61);
           border-radius: 5px;
       }

      QMenu {
           color: rgb(152, 160, 175);
           background-color: rgb(30, 34, 39);
      }
      QMenu::item:selected {
          background-color: rgb(52, 56, 61);
      }
    )");

    // Initialize menu item.
    QMenu *menu = menubar->addMenu("File");
    menu->addAction("New Text File");
    menu->addAction("New File");
    menu->addAction("New Window");
    menu->addAction("Open File");
    menu->addAction("Open Directory");
    menu = menubar->addMenu("Edit");
    menu->addAction("Undo");
    menu->addAction("Redo");
    menu->addAction("Cut");
    menu->addAction("Copy");
    menu->addAction("Paste");
    menu = menubar->addMenu("Selected Zone");
    menu->addAction("Select All");
    menu->addAction("Expand Select Zone");
    menu->addAction("Collapse Select Zone");
    menu->addAction("Add Cursor");
    menu->addAction("Select Line Mode");
    menu = menubar->addMenu("View");
    menu->addAction("Command Pallete");
    menu->addAction("Open View");
    menu->addAction("Shape");
    menu->addAction("Edit Layout");

    // Add widget to title bar.
    // "clickable widget" property should be true if you want this widget to work on the title bar.
    menubar->setProperty("clickable widget", true);
    w.getCustomTitlebarLayout().addWidget(menubar);

    // Add a non-clickable element to make the area moveable for frameless window.
    // All you need to do is set the "clickable widget" property to false.
    // Then user can move frameless window by clicking area of non_clickable widget.
    QWidget *non_clickable = new QWidget(&w);
    non_clickable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    non_clickable->setProperty("clickable widget", false);
    w.getCustomTitlebarLayout().addWidget(non_clickable);

    // Set the Icon here.
    w.setWindowIcon(QIcon(":/icon/ApplicationIcon.png"));

    // Set window size.
    int window_width = 1024, window_height = 768;
    w.setGeometry(QApplication::primaryScreen()->geometry().width() / 2 - window_width / 2,
                  QApplication::primaryScreen()->geometry().height() / 2 - window_height / 2,
                  window_width, window_height);
    w.setMinimumSize(400, 300);

    //! MainWindow class provides getTitlebarWidget() function.
    //! It return title bar widget.
    // Set titlebar widget palette.
    auto pal = w.getTitlebarWidget().palette();
    pal.setColor(QPalette::Window, QColor(30, 34, 39));
    w.getTitlebarWidget().setAutoFillBackground(true);
    w.getTitlebarWidget().setPalette(pal);

    //! MainWindow class provides getContentWidget() function.
    //! It return main content widget.
    // Set main content widget palette.
    pal = w.getContentWidget().palette();
    pal.setColor(QPalette::Window, QColor(35, 39, 46));
    w.getContentWidget().setAutoFillBackground(true);
    w.getContentWidget().setPalette(pal);

    // Add layout to main content widget.
    QVBoxLayout *main_widget_layout = new QVBoxLayout(&w);
    w.getContentWidget().setLayout(main_widget_layout);

    // Add some button.
    QPushButton *btn = new QPushButton("This is Button!", &w.getContentWidget());
    btn->setFixedSize(180, 35);
    main_widget_layout->addWidget(btn);
    main_widget_layout->setAlignment(btn, Qt::AlignHCenter);

    w.show();

    return app.exec();
}
