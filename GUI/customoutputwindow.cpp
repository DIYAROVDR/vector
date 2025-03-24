#include "customoutputwindow.h"
#include <vtkObjectFactory.h>
#include <QString>

vtkStandardNewMacro(CustomOutputWindow);

void CustomOutputWindow::SetTextEdit(QPlainTextEdit* editor)
{
    this->textEdit = editor;
}

void CustomOutputWindow::DisplayText(const char* text)
{
    if (textEdit)
    {
        textEdit->appendPlainText(QString::fromUtf8(text));
    }
}
