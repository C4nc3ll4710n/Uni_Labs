#include "xmlwindow.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsTextItem>

XmlWindow::XmlWindow(QWidget *parent) : QWidget(parent) {
    scene = new QGraphicsScene();
    view = new QGraphicsView();
    layout = new QVBoxLayout(this);
    menuFile = new QMenu(tr("File"), this);
    menuBar = new QMenuBar();
    menuBar->addMenu(menuFile);
    menuFile->addAction(tr("Open"), this, SLOT(open()), QKeySequence(Qt::CTRL + Qt::Key_O));
    menuFile->addAction(tr("Exit"), this, SLOT(exit()), QKeySequence(Qt::CTRL + Qt::Key_W));
    layout->setMenuBar(menuBar);
    layout->addWidget(view);
    central = new QWidget();
    central->setLayout(layout);
    central->setWindowTitle("Check graph");
    central->show();
}

void XmlWindow::open() {
    QXmlStreamReader xmlReader;
    QFile file = QFileDialog::getOpenFileName(this, tr("Open file"), tr(""), tr("XML(*.xml)"));
    file.open(QIODevice::ReadOnly);
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    int type = 0;
    double x = 0;
    double y = 0;
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;
    double width = 0;
    double height = 0;
    QString text;
    while (!xmlReader.atEnd()) {
        if (xmlReader.name() == "GraphicsItem") {
            foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                if (attr.name().toString() == "type")
                    type = attr.value().toInt();
                else if (attr.name().toString() == "x")
                    x = attr.value().toDouble();
                else if (attr.name().toString() == "y")
                    y = attr.value().toDouble();
                else if (attr.name().toString() == "x1")
                    x1 = attr.value().toDouble();
                else if (attr.name().toString() == "y1")
                    y1 = attr.value().toDouble();
                else if (attr.name().toString() == "x2")
                    x2 = attr.value().toDouble();
                else if (attr.name().toString() == "y2")
                    y2 = attr.value().toDouble();
                else if (attr.name().toString() == "width")
                    width = attr.value().toDouble();
                else if (attr.name().toString() == "height")
                    height = attr.value().toDouble();
                else if (attr.name().toString() == "text")
                    text = attr.value().toString();
            }
            if (type == 6)
                scene->addLine(x1, y1, x2, y2);
            else if (type == 3)
                scene->addRect(x, y, width, height);
            else if (type == 8) {
                QGraphicsTextItem *textItem = new QGraphicsTextItem();
                textItem->setHtml(text);
                textItem->setPos(x, y);
                scene->addItem(textItem);
            }
        }
        xmlReader.readNext();
    }
    file.close();
    view->setScene(scene);
    central->setWindowTitle(QFileInfo(file).fileName());
}

void XmlWindow::exit() {
    central->close();
}
