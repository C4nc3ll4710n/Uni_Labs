#ifndef CALLGRAPH_H
#define CALLGRAPH_H

#include <QGraphicsScene>
#include <QTabWidget>
#include <QFile>
#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>

class CallGraph : public QWidget {
    Q_OBJECT
    QGraphicsScene *scene;
    QGraphicsView *view;
    QMenu *menuFile;
    QMenuBar *menuBar;
    QVBoxLayout *layout;
    QWidget *central;
    const int between_x = 60;
    const int between_y = 20;

public:
    CallGraph();
    void drawGraph(QFile input_file);

private slots:
    void save();
    void exit();
};

#endif // CALLGRAPH_H
