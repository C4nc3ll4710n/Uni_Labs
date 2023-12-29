#ifndef XMLWINDOW_H
#define XMLWINDOW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>

class XmlWindow : public QWidget {
    Q_OBJECT
    QGraphicsScene *scene;
    QGraphicsView *view;
    QMenu *menuFile;
    QMenuBar *menuBar;
    QVBoxLayout *layout;
    QWidget *central;

public:
    explicit XmlWindow(QWidget *parent = nullptr);

private slots:
    void open();
    void exit();
};

#endif // XMLWINDOW_H
