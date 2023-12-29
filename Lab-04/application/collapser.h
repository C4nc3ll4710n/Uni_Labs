#ifndef COLLAPSER_H
#define COLLAPSER_H

#include <QObject>
#include <QDir>
#include <QApplication>
#include <QTextEdit>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextStream>
#include "highlighter.h"

class Collapser {
private:
    Highlighter *highlighter;
    QVector<QPair<QString, QPair<QString, QPair<int, int>>>> blocks;

public:
    void parse(QString filename, int state);
    void doCodeBlock(bool is_empty, QTextEdit *ed);
};

#endif // COLLAPSER_H
