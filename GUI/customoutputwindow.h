#ifndef CUSTOMOUTPUTWINDOW_H
#define CUSTOMOUTPUTWINDOW_H

#include <vtkOutputWindow.h>
#include <vtkObjectFactory.h>
#include <QString>
#include <QPlainTextEdit>

class CustomOutputWindow : public vtkOutputWindow {
public:
    static CustomOutputWindow* New();
    vtkTypeMacro(CustomOutputWindow, vtkOutputWindow);

    void SetTextEdit(QPlainTextEdit* editor);
    void DisplayText(const char* text) override;

private:
    QPlainTextEdit* textEdit = nullptr;
};

#endif // CUSTOMOUTPUTWINDOW_H
