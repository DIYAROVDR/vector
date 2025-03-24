#include "scriptsyntaxhighlighter.h"

#include <QTextDocument>

ScriptSyntaxHighlighter::ScriptSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {

    // Формат для ключевых слов
    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywordPatterns = {
        "\\band\\b", "\\bas\\b", "\\bassert\\b", "\\bbreak\\b", "\\bclass\\b", "\\bcontinue\\b",
        "\\bdef\\b", "\\bdel\\b", "\\belif\\b", "\\belse\\b", "\\bexcept\\b", "\\bfinally\\b",
        "\\bfor\\b", "\\bfrom\\b", "\\bglobal\\b", "\\bif\\b", "\\bimport\\b", "\\bin\\b",
        "\\bis\\b", "\\blambda\\b", "\\bnot\\b", "\\bor\\b", "\\bpass\\b", "\\braise\\b",
        "\\breturn\\b", "\\btry\\b", "\\bwhile\\b", "\\bwith\\b", "\\byield\\b"
    };

    for (const QString &pattern : keywordPatterns) {
        HighlightingRule rule;
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Формат для строк
    stringFormat.setForeground(Qt::darkRed);
    HighlightingRule stringRule;
    stringRule.pattern = QRegularExpression("\".*\"|'.*'"); // строки в кавычках
    stringRule.format = stringFormat;
    highlightingRules.append(stringRule);

    // Формат для комментариев
    commentFormat.setForeground(Qt::darkGreen);
    HighlightingRule commentRule;
    commentRule.pattern = QRegularExpression("#[^\n]*"); // комментарии Python
    commentRule.format = commentFormat;
    highlightingRules.append(commentRule);

    // Формат для чисел
    numberFormat.setForeground(Qt::darkMagenta);
    HighlightingRule numberRule;
    numberRule.pattern = QRegularExpression("\\b\\d+\\b"); // числа
    numberRule.format = numberFormat;
    highlightingRules.append(numberRule);

    // Формат для функций
    functionFormat.setForeground(Qt::darkCyan);
    functionFormat.setFontItalic(true);
    HighlightingRule functionRule;
    functionRule.pattern = QRegularExpression("\\b[A-Za-z_]+(?=\\()"); // функции
    functionRule.format = functionFormat;
    highlightingRules.append(functionRule);
}

void ScriptSyntaxHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
