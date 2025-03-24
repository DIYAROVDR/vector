#ifndef SCRIPTSYNTAXHIGHLIGHTER_H
#define SCRIPTSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

// Класс для подсветки синтаксиса Python
class ScriptSyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit ScriptSyntaxHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat functionFormat;
};

#endif // SCRIPTSYNTAXHIGHLIGHTER_H
