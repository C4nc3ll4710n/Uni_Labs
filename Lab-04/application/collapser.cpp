#include "collapser.h"

void Collapser::parse(QString filename, int state = 0) {
    /* 0 - save
     * 1 - open
     * 2 - close */
    QString command = "cscope -bk " + filename;
    QByteArray byte_arr = command.toLocal8Bit();
    const char *cstr_command = byte_arr.data();
    system(cstr_command);
    QString path = QDir::currentPath() + "/cscope.out";
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString prev = "";
    for (QString cur = in.readLine(); !cur.isNull(); cur = in.readLine()) {
        QRegExp regex("[$][A-Za-z0-9_-]+");
        int pos = regex.indexIn(cur);
        QString func = regex.cap(0).mid(1);
        if (pos != -1) {
            int prev_pos = prev.indexOf(QRegExp("[0-9]+"));
            int prev_end_pos = prev.indexOf(QRegExp("[^ \n]"), pos + 1);
            QString line = prev.mid(prev_pos, prev_end_pos - prev_pos);
            bool ok;
            int num = line.toInt(&ok);
            if (ok) {
                if ((state < 0) || (state > 2))
                    state = 0;
                if (state == 0) {
                    bool is_repeat = false;
                    for (int i = 0; i < blocks.size(); i++) {
                        if (blocks[i].first == file.fileName() && blocks[i].second.first == func && blocks[i].second.second.first == num) {
                            is_repeat = true;
                        }
                    }
                    if (!is_repeat)
                        blocks.append(qMakePair(filename, qMakePair(func, qMakePair(num, -1))));
                }
                else if (state == 1) {
                    blocks.append(qMakePair(filename, qMakePair(func, qMakePair(num, -1))));
                }
                else if (state == 2) {
                    for (int i = 0; i < blocks.size(); i++)
                        if (blocks[i].first == file.fileName() && blocks[i].second.first == func && blocks[i].second.second.first == num)
                            blocks.remove(i);
                }
            }
        }
        if (cur[1] == '}' && (state == 0 || state == 1)) {
            int prev_pos = prev.indexOf(QRegExp("[0-9]+"));
            int prev_end_pos = prev.indexOf(QRegExp("[^ \n]"), pos);
            QString line = prev.mid(prev_pos, prev_end_pos - prev_pos);
            bool ok;
            int num = line.toInt(&ok);
            int ind = blocks.size() - 1;
            while (ind >= 0 && blocks[ind].second.second.second != -1)
                ind--;
            if (ind == -1)
                blocks[0].second.second.second = num;
            else
                blocks[ind].second.second.second = num;
        }
        prev = cur;
    }
}
void Collapser::doCodeBlock(bool is_empty, QTextEdit *ed) {
    if (!is_empty) {
        QTextCursor cursor(ed->textCursor());
        int start = cursor.blockNumber() + 1;
        int end = -1;
        for (int i = 0; i < blocks.size(); i++)
            if (blocks[i].second.second.first == start)
                end = blocks[i].second.second.second;
        if (end != -1) {
            for (int i = start; i < end - 1; i++) {
                cursor.movePosition(QTextCursor::Down);
                if (cursor.block().isVisible())
                    cursor.block().setVisible(false);
                else
                    cursor.block().setVisible(true);
            }
            QFont font;
            font.setFamily("Courier");
            font.setFixedPitch(true);
            font.setPointSize(10);
            ed->setFont(font);
            highlighter = new Highlighter(ed->document());
        }
    }
}
