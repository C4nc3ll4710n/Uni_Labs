#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "highlighter.h"
#include "collapser.h"
#include "callgraph.h"
#include "xmlwindow.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
class QSessionManager;
class QListWidget;
class QHBoxLayout;
class QTextCursor;
class QTextBlock;
class QGraphicsScene;
class QGraphicsView;
class QGraphicsRectItem;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
    QMenu *menuFile;
    QMenu *menuTools;
    QVector<QPair<QString, QWidget*>> tabs;
    QListWidget *reviewer;
    QTabWidget *editor;
    QWidget *central;
    QHBoxLayout *layout;
    QString tab_title = "untitled";
    QVector<QPair<QString, QPair<QString, int>>> review_lines;
    QVector<QRegExp> navigationRules;

    QAction *action;
    QTextEdit *textEdit;
    Highlighter *highlighter;
    Collapser collapser;
    CallGraph callGraph;
    QString curFile;
    XmlWindow xmlWindow;

    void readSettings();
    void writeSettings();
    void save(QWidget *current);
    bool maybeSave();
    bool maybeSave(QWidget *current);
    void fillRules(QVector<QRegExp> &navigationRules);

public:
    MainWindow();
    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void drawGraph();
    void doCodeBlock();
    void onListDoubleClicked(const QModelIndex& index);
    void newTab();
    void closeTab();
    void save();
    void open();
    void checkTabs();

#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif 
};

#endif
