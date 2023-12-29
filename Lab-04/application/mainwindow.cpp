#include <QtWidgets>
#include <QDir>
#include <QGraphicsScene>
#include "mainwindow.h"

MainWindow::MainWindow() {
    reviewer = new QListWidget(this);
    editor = new QTabWidget(this);
    central = new QWidget(this);
    layout = new QHBoxLayout(this);
    layout->addWidget(reviewer, 1);
    layout->addWidget(editor, 2);
    central->setLayout(layout);
    setCentralWidget(central);
    central->setWindowTitle("Text editor");

    QWidget *t = new QTextEdit(this);
    tabs.push_back(qMakePair(QString(""), t));
    editor->addTab(t, tab_title);
    menuFile = new QMenu(tr("File"), this);
    menuBar()->addMenu(menuFile);
    menuFile->addAction(tr("New tab"), this, SLOT(newTab()), QKeySequence(Qt::CTRL + Qt::Key_T));
    menuFile->addAction(tr("Close tab"), this, SLOT(closeTab()), QKeySequence(Qt::CTRL + Qt::Key_W));
    menuFile->addAction(tr("Save"), this, SLOT(save()), QKeySequence(Qt::CTRL + Qt::Key_S));
    menuFile->addAction(tr("Open"), this, SLOT(open()), QKeySequence(Qt::CTRL + Qt::Key_O));
    menuFile->addAction(tr("Collapse/Expand"), this, SLOT(doCodeBlock()), QKeySequence(Qt::CTRL + Qt::Key_E));

    menuTools = new QMenu(tr("Tools"), this);
    menuBar()->addMenu(menuTools);
    action = new QAction("Call graph");
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    action->setEnabled(false);
    connect(action, SIGNAL(triggered()), this, SLOT(drawGraph()));
    menuTools->addAction(action);

    connect(reviewer, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListDoubleClicked(QModelIndex)));
    connect(editor, SIGNAL(currentChanged(int)), this, SLOT(checkTabs()));
    readSettings();
    fillRules(navigationRules);

#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif
}

void MainWindow::checkTabs() {
    bool flag = false;
    if (tabs.size() != 0) {
        QWidget *current = editor->currentWidget();
        size_t i = 0;

        bool end_flag = true;
        while (end_flag && tabs[i].second != current) {
            i++;
            if ((int)i + 1 > tabs.size())
                end_flag = false;
        }
        if (end_flag && tabs[i].first.size() != 0) {
            QFile file(tabs[i].first);
            QFileInfo file_info(file);
            if (file_info.suffix() == "cpp" || file_info.suffix() == "h")
                flag = true;
        }
    }
    action->setEnabled(flag);
}

void MainWindow::drawGraph() {
    QWidget *current = editor->currentWidget();
    size_t i = 0;
    while (tabs[i].second != current)
        i++;
    callGraph.drawGraph(QFile(tabs[i].first));
}

void MainWindow::fillRules(QVector<QRegExp> &navigationRules) {
    QStringList keywordPatterns;
    keywordPatterns << "\\bbool [A-Za-z0-9_]+[^;{\n]*" << "\\bchar [A-Za-z0-9_]+[^;{\n]*" << "\\bclass [A-Za-z0-9_]+[^;{\n]*" << "\\bconst [A-Za-z0-9_]+[^;{\n]*"
                    << "\\bdouble [A-Za-z0-9_]+[^;{\n]*" << "\\benum [A-Za-z0-9_]+[^;{\n]*" << "\\bfriend [A-Za-z0-9_]+[^;{\n]*" << "\\bint [A-Za-z0-9_]+[^;{\n]*"
                    << "\\blong [A-Za-z0-9_]+[^;{\n]*" << "\\boperator [A-Za-z0-9_]+[^;{\n]*" << "\\bshort [A-Za-z0-9_]+[^;{\n]*" << "\\bsigned [A-Za-z0-9_]+[^;{\n]*"
                    << "\\bstatic [A-Za-z0-9_]+[^;{\n]*" << "\\bstruct [A-Za-z0-9_]+[^;{\n]*" << "\\btypedef [A-Za-z0-9_]+[^;{\n]*"
                    << "\\bunion [A-Za-z0-9_]+[^;{\n]*" << "\\bunsigned [A-Za-z0-9_]+[^;{\n]*" << "\\bvirtual [A-Za-z0-9_]+[^;{\n]*" << "\\bvoid [A-Za-z0-9_]+[^;{\n]*";
    foreach (const QString &pattern, keywordPatterns)
        navigationRules.append(QRegExp(pattern));
    navigationRules.append(QRegExp("\\bQ[A-Za-z] [A-Za-z0-9_]+[^>;{\n]*"));
}

void MainWindow::doCodeBlock() {
    collapser.doCodeBlock(tabs.empty(), (QTextEdit*)editor->currentWidget());
}

void MainWindow::onListDoubleClicked(const QModelIndex& index) {
    if (!index.isValid())
        return;
    if (QListWidget* reviewer = dynamic_cast<QListWidget*>(sender())) {
        QString current = reviewer->currentItem()->text();
        for (int i = 0; i < review_lines.size(); i++) {
            if (review_lines[i].second.first == current) {
                int pos = current.indexOf(" — ");
                QString name = current.mid(pos + 3, current.size() - 3 - pos);
                size_t i = 0;
                while (tabs[i].first.split("/").last() != name)
                    i++;
                editor->setCurrentWidget(tabs[i].second);
                QWidget *current = editor->currentWidget();
                QTextEdit *ed = (QTextEdit*)current;
                QTextCursor tmpCursor = ed->textCursor();
                tmpCursor.setPosition(review_lines[i].second.second);
                ed->setTextCursor(tmpCursor);
            }
        }
    }
}

void MainWindow::newTab() {
    QWidget *t = new QTextEdit(this);
    tabs.push_back(qMakePair(QString(""), t));
    editor->addTab(t, tab_title);
}

void MainWindow::closeTab() {
    if (tabs.size() != 0) {
        QWidget *current = editor->currentWidget();
        QTextEdit *ed = (QTextEdit*)current;
        if (maybeSave()) {
            size_t i = 0;
            while (tabs[i].second != current)
                i++;
            writeSettings();
            if (tabs[i].first.size() != 0 || !ed->document()->isModified()) {
                QFile file(tabs[i].first);
                QFileInfo file_info(file);
                if (file_info.suffix() == "cpp" || file_info.suffix() == "h") {
                    collapser.parse(file.fileName(), 2);
                    foreach (const QRegExp &rule, navigationRules) {
                        int ind = 0;
                        while ((ind = rule.indexIn(ed->toPlainText(), ind)) != -1) {
                            QString current = rule.cap(0);
                            QList<QListWidgetItem*> items = reviewer->findItems(current +  " — " + QFileInfo(file).fileName(), Qt::MatchExactly);
                            reviewer->removeItemWidget(items[0]);
                            delete items[0];
                            for (int i = 0; i < review_lines.size(); i++)
                                if (review_lines[i].first == file.fileName() && review_lines[i].second.first == current && review_lines[i].second.second == ind)
                                    review_lines.remove(i);
                            ind += rule.matchedLength();
                        }
                    }
                }
                editor->removeTab(i);
                tabs.remove(i);
            }
            else
                editor->setTabText(i, tab_title);
        }
    }
}

void MainWindow::save() {
    QWidget *current = editor->currentWidget();
    save(current);
}

void MainWindow::save(QWidget *current) {
    QTextEdit *ed = (QTextEdit*)current;
    size_t i = 0;
    while (tabs[i].second != current)
        i++;
    if (tabs[i].first.size() == 0)
        tabs[i].first = QFileDialog::getSaveFileName(this, tr("Save file"));
    QFile file(tabs[i].first);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream << ed->toPlainText();
    file.close();
    QFileInfo file_info(file);
    editor->setTabText(i, QString(file_info.fileName()));
    tabs[i].second = current;

    if (file_info.suffix() == "cpp" || file_info.suffix() == "h") {
        QFont font;
        font.setFamily("Courier");
        font.setFixedPitch(true);
        font.setPointSize(10);
        ed->setFont(font);
        highlighter = new Highlighter(ed->document());

        collapser.parse(file.fileName(), 0);
        foreach (const QRegExp &rule, navigationRules) {
            int ind = 0;
            while ((ind = rule.indexIn(ed->toPlainText(), ind)) != -1) {
                QString current = rule.cap(0) + " — " + file_info.fileName();
                bool is_repeat = false;
                for (int i = 0; i < review_lines.size(); i++)
                    if (review_lines[i].first == file.fileName() && review_lines[i].second.first == current && review_lines[i].second.second == ind)
                        is_repeat = true;
                if (!is_repeat) {
                    reviewer->addItem(current);
                    review_lines.append(qMakePair(file.fileName(), qMakePair(current, ind)));
                }
                ind += rule.matchedLength();
            }
        }
    }
    checkTabs();
}

void MainWindow::open() {
    QFile file = QFileDialog::getOpenFileName(this);
    file.open(QIODevice::ReadWrite);
    QFileInfo file_info(file);
    QTextEdit *t = new QTextEdit(this);
    if (file_info.suffix() == "cpp" || file_info.suffix() == "h") {
        QFont font;
        font.setFamily("Courier");
        font.setFixedPitch(true);
        font.setPointSize(10);
        t->setFont(font);
        highlighter = new Highlighter(t->document());
        t->setPlainText(file.readAll());

        collapser.parse(file.fileName(), 1);
        foreach (const QRegExp &rule, navigationRules) {
            int ind = 0;
            while ((ind = rule.indexIn(t->toPlainText(), ind)) != -1) {
                QString current = rule.cap(0) + " — " + QFileInfo(file).fileName();
                reviewer->addItem(current);
                review_lines.append(qMakePair(file.fileName(), qMakePair(current, ind)));
                ind += rule.matchedLength();
            }
        }
    }
    else {
        QByteArray data;
        data = file.readAll();
        t->setText(data);
    }
    tabs.push_back(qMakePair(QString(file.fileName()), t));
    editor->addTab(t, QFileInfo(file).fileName());
}

void MainWindow::closeEvent(QCloseEvent *event) {
    for (int i = 0; i < tabs.size(); i++) {
        if (maybeSave(tabs[i].second)) {
            writeSettings();
            event->accept();
        }
        else {
            event->ignore();
        }
    }
}

bool MainWindow::maybeSave() {
    QWidget *current = editor->currentWidget();
    return maybeSave(current);
}

bool MainWindow::maybeSave(QWidget *current) {
    QTextEdit *ed = (QTextEdit*)current;
    if (!ed->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        save(current);
        return true;
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::readSettings() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    }
    else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager)
{
    if (manager.allowsInteraction()) {
        manager.cancel();
    }
    else {
        // Non-interactive: save without asking
        if (textEdit->document()->isModified())
            save();
    }
}
#endif
