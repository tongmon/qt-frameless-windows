import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 1024
    height: 768
    minimumWidth: 400
    minimumHeight: 300
    title: qsTr("FrameLess Window")
    flags: Qt.Window
    
    // Border width for window resizing
    // This is used in the WinQuickWindow.cpp file.
    property int resizeBorderWidth: 6

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: titleBar
            color: "#1f0830"

            Layout.fillWidth: true
            Layout.preferredHeight: 35  // Title Bar height 
            Layout.alignment: Qt.AlignTop

            RowLayout {
                anchors.fill: parent
                spacing: 0

                // You can customize the menu bar as you like.
                component TitleBarMenuItem : MenuItem {
                    id: menuItem
                    implicitHeight: titleBar.height
                    contentItem: Text {
                        text: menuItem.text
                        font: menuItem.font
                        color: active ? (menuItem.highlighted ? "#ffffff" : Qt.rgba(0.8, 0.8, 0.8, 1.0)) : Qt.rgba(0.5, 0.5, 0.5, 1.0)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                    background: Rectangle {
                        anchors.fill: parent
                        opacity: menuItem.highlighted ? 0.7 : 1.0
                        color: "#170624"
                    }
                }

                MenuBar {
                    id: titleMenuBar
                    Layout.alignment: Qt.AlignLeft

                    Menu {
                        title: "File"
                        Action {
                            text: "New Text File"
                        }
                        Action {
                            text: "New File"
                        }
                        Action {
                            text: "New Window"
                        }
                        MenuSeparator {
                            contentItem: Rectangle {
                                implicitHeight: 1
                                color: "#21be2b"
                            }
                            background: Rectangle {
                                color: "#170624"
                            }
                        }
                        Action {
                            text: "Open File"
                        }
                        Action {
                            text: "Open Directory"
                        }

                        delegate: TitleBarMenuItem { }
                    }
                    Menu {
                        title: "Edit"
                        Action {
                            text: "Undo"
                        }
                        Action {
                            text: "Redo"
                        }
                        Action {
                            text: "Cut"
                        }
                        Action {
                            text: "Copy"
                        }
                        Action {
                            text: "Paste"
                        }

                        delegate: TitleBarMenuItem { }
                    }
                    Menu {
                        title: "Selected Zone"
                        Action {
                            text: "Select All"
                        }
                        Action {
                            text: "Expand Select Zone"
                        }
                        Action {
                            text: "Collapse Select Zone"
                        }
                        Action {
                            text: "Add Cursor"
                        }
                        Action {
                            text: "Select Line Mode"
                        }

                        delegate: TitleBarMenuItem { }
                    }
                    Menu {
                        title: "View"
                        Action {
                            text: "Command Pallete"
                        }
                        Action {
                            text: "Open View"
                        }
                        Action {
                            text: "Shape"
                        }
                        Action {
                            text: "Edit Layout"
                        }

                        delegate: TitleBarMenuItem { }
                    }

                    delegate: MenuBarItem {
                        id: menuBarItem
                        contentItem: Text {
                            text: menuBarItem.text
                            font: menuBarItem.font
                            color: active ? Qt.rgba(0.8, 0.8, 0.8, 1.0) : Qt.rgba(0.5, 0.5, 0.5, 1.0)
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                        background: Rectangle {
                            implicitHeight: titleBar.height
                            color: menuBarItem.highlighted ? Qt.rgba(1.0, 1.0, 1.0, 0.2) : "transparent"
                        }
                    }

                    background: Rectangle {
                        implicitHeight: titleBar.height
                        color: "transparent"
                        Rectangle {
                            color: "#21be2b"
                            width: parent.width
                            height: 1
                            anchors.bottom: parent.bottom
                        }
                    }
                }

                // This is the area where the user can click to move the window.
                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    MouseArea {
                        anchors.fill: parent

                        // When you press this area, the window becomes movable.
                        onPressed: {
                            applicationWindow.startSystemMove()
                        }

                        // When you double-click this area, the window state changes as well.
                        onDoubleClicked: {
                            applicationWindow.visibility = maximumButton.checked ? Window.Windowed : Window.Maximized
                        }
                    }
                }

                Button {
                    id: minimumButton
                    Layout.preferredWidth: 46
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignRight
                    background: Rectangle {
                        color: parent.down ? Qt.rgba(1.0, 1.0, 1.0, 0.4) : (parent.hovered ? Qt.rgba(1.0, 1.0, 1.0, 0.2) :
                                                                            Qt.rgba(1.0, 1.0, 1.0, 0.0))
                    }
                    Image {
                        source: active ? "qrc:/icon/Minimize.png" : "qrc:/icon/MinimizeDeactivated.png"
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                    }

                    // Trigger onMinimizeButtonClicked function implemented on C++ side.
                    onClicked: {
                        cppConnector.onMinimizeButtonClicked()
                    }
                }

                Button {
                    id: maximumButton
                    objectName: "maximumButton"
                    Layout.preferredWidth: 46
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignRight
                    checkable: true
                    checked: false
                    background: Rectangle {
                        color: parent.down ? Qt.rgba(1.0, 1.0, 1.0, 0.4) : (parent.hovered ? Qt.rgba(1.0, 1.0, 1.0, 0.2) :
                                                                                             Qt.rgba(1.0, 1.0, 1.0, 0.0))
                    }
                    Image {
                        source: maximumButton.checked ? (active ? "qrc:/icon/Restore.png" : "qrc:/icon/RestoreDeactivated.png") :
                                                        (active ? "qrc:/icon/Maximize.png" : "qrc:/icon/MaximizeDeactivated.png")
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                    }

                    // Trigger onMaximizeButtonClicked function implemented on C++ side.
                    onClicked: {
                        cppConnector.onMaximizeButtonClicked()
                    }
                }

                Button {
                    id: closeButton
                    Layout.preferredWidth: 46
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignRight
                    background: Rectangle {
                        color: parent.down ? Qt.rgba(0.78, 0.16, 0.184, 0.6) : (parent.hovered ? Qt.rgba(0.86, 0.16, 0.184, 0.9) :
                                                                                                 Qt.rgba(1.0, 1.0, 1.0, 0.0))
                    }
                    Image {
                        source: closeButton.hovered ? "qrc:/icon/CloseHoverOrPressed.png" :
                                                      (active ? "qrc:/icon/Close.png" : "qrc:/icon/CloseDeactivated.png")
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                    }

                    // Trigger onMaximizeButtonClicked function implemented on C++ side.
                    onClicked: {
                        cppConnector.onCloseButtonClicked()
                    }
                }
            }
        }

        Rectangle {
            id: mainContent
            color: "#280a3d"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignBottom

            Button {
                text: "This is Button!"
                anchors.centerIn: parent
            }
        }
    }

    // Move window to center.
    Component.onCompleted: {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2
    }
}
