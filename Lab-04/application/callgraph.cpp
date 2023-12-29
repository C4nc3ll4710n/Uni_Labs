#include "callgraph.h"
#include <QDir>
#include <QTextStream>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QXmlStreamWriter>
#include <QFileDialog>

CallGraph::CallGraph() {
    scene = new QGraphicsScene();
    view = new QGraphicsView();
    layout = new QVBoxLayout(this);
    menuFile = new QMenu(tr("File"), this);
    menuBar = new QMenuBar();
    menuBar->addMenu(menuFile);
    menuFile->addAction(tr("Save"), this, SLOT(save()), QKeySequence(Qt::CTRL + Qt::Key_S));
    menuFile->addAction(tr("Exit"), this, SLOT(exit()), QKeySequence(Qt::CTRL + Qt::Key_W));
    layout->setMenuBar(menuBar);
    layout->addWidget(view);
    central = new QWidget();
    central->setLayout(layout);
    central->setWindowTitle("Call graph");
}

void CallGraph::save() {
    QFile file = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("XML(*.xml)"));
    if (!file.open(QIODevice::WriteOnly))
        return;
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("SceneData");
    xmlWriter.writeAttribute("version", "v1.0");
    xmlWriter.writeStartElement("GraphicsItemList");
    foreach (QGraphicsItem* item, scene->items()) {
        xmlWriter.writeStartElement("GraphicsItem");
        xmlWriter.writeAttribute("type", QString::number(item->type()));
        if (item->type() == 8) {
            item->data(0);
            xmlWriter.writeAttribute("x", QString::number(item->x()));
            xmlWriter.writeAttribute("y", QString::number(item->y()));
            xmlWriter.writeAttribute("text", item->data(0).toString());
        }
        else if (item->type() == 3) {
            QRectF rect = item->boundingRect();
            xmlWriter.writeAttribute("x", QString::number(rect.x()));
            xmlWriter.writeAttribute("y", QString::number(rect.y()));
            xmlWriter.writeAttribute("width", QString::number(rect.width()));
            xmlWriter.writeAttribute("height", QString::number(rect.height()));
        }
        else {
            QPointF point = item->shape().currentPosition();
            QRectF rect = item->boundingRect();
            double start_y = 0;
            if (rect.y() + rect.height() - 1 < point.y() && point.y() < rect.y() + rect.height() + 1)
                start_y = rect.y();
            else
                start_y = rect.y() + rect.height();
            xmlWriter.writeAttribute("x1", QString::number(rect.x()));
            xmlWriter.writeAttribute("y1", QString::number(start_y));
            xmlWriter.writeAttribute("x2", QString::number(rect.x() + rect.width()));
            xmlWriter.writeAttribute("y2", QString::number(point.y()));
        }
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    central->setWindowTitle(QFileInfo(file).fileName());
}

void CallGraph::exit() {
    central->close();
}

void CallGraph::drawGraph(QFile input_file) {
    QString command = "cflow --output=cflow.txt " + input_file.fileName();
    QByteArray byte_arr = command.toLocal8Bit();
    const char *cstr_command = byte_arr.data();
    system(cstr_command);
    QString path = QDir::currentPath() + "/cflow.txt";

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    int max_level = 1;

    QVector<int> widths;
    for (int level = 0; level < max_level; level++) {
        widths.push_back(0);
        file.open(QIODevice::ReadOnly);
        for (QString cur = in.readLine(); !cur.isNull(); cur = in.readLine()) {
            int spaces = 0;
            while (cur[spaces] == ' ')
                spaces++;
            if (spaces > (max_level - 1) * 4)
                max_level = spaces / 4 + 1;
            QGraphicsTextItem *textItem = new QGraphicsTextItem();
            textItem->setHtml(cur);
            textItem->setTextWidth(textItem->boundingRect().width());
            QRectF *rect = new QRectF(textItem->boundingRect());
            rect->moveCenter(textItem->boundingRect().center());
            int cur_width = rect->width();
            if (spaces / 4 == level && cur_width > widths[level])
                widths[level] = cur_width;
        }
        file.close();
    }

    int global_x = 0;
    for (int level = 0; level < max_level; level++) {
        file.open(QIODevice::ReadOnly);
        int rect_num = 0;
        int parent_x = 0;
        int parent_y = 0;
        int prev_spaces = 0;
        int previous_level = 0;
        bool flag = false;
        for (QString cur = in.readLine(); !cur.isNull(); cur = in.readLine()) {
            int spaces = 0;
            while (cur[spaces] == ' ')
                spaces++;
            if (level == spaces / 4) {
                QGraphicsTextItem *textItem = new QGraphicsTextItem();
                textItem->setHtml(cur);
                textItem->setData(0, cur);
                textItem->setTextWidth(textItem->boundingRect().width());
                QRectF *rect = new QRectF(textItem->boundingRect());
                rect->moveCenter(textItem->boundingRect().center());
                if (prev_spaces + 4 == spaces) {
                    flag = true;
                    parent_x = global_x;
                    parent_y = previous_level * (between_y + rect->height()) + rect->height() / 2;
                }
                rect_num++;
                int cur_x = global_x + between_x;
                int cur_y = rect_num * (between_y + rect->height());

                textItem->setPos(cur_x, cur_y);
                scene->addItem(textItem);
                if (level + 1 == max_level)
                    scene->addRect(cur_x, cur_y, rect->width(), rect->height());
                else
                    scene->addRect(cur_x, cur_y, widths[level], rect->height());
                if (level != 0)
                    scene->addLine(parent_x, parent_y, cur_x, cur_y + rect->height() / 2);
            }
            else if (flag)
                flag = false;
            if (spaces / 4 == level - 1)
                previous_level++;
            prev_spaces = spaces;
        }
        global_x += widths[level] + between_x;
        file.close();
    }

    view->setScene(scene);
    central->show();
}
