#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPainter>
#include <QTextBlock>
#include <QPlainTextEdit>

#include "scriptsyntaxhighlighter.h"

class LineNumberArea;

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);

    int lineNumberAreaWidth();
    void lineNumberAreaPaintEvent(QPaintEvent *event);

protected:
    void wheelEvent(QWheelEvent *event) override;  // Добавляем новое событие для обработки колеса мыши
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);
    void highlightCurrentLine();

private:
    QWidget* lineNumberArea;
    ScriptSyntaxHighlighter* scriptSyntax;
};

class LineNumberArea : public QWidget {
public:
    explicit LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor) {}

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:

    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor* codeEditor;
};

#endif // CODEEDITOR_H
