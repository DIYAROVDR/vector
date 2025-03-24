#include "pythonmodule.h"

PythonModule::PythonModule() {
    Py_Initialize();
    mainModule = PyImport_AddModule("__main__");
    globalDict = PyModule_GetDict(mainModule);
}

void PythonModule::run(const QString& script) {
    vtkPythonInterpreter::RunSimpleString(script.toStdString().c_str());
}

void PythonModule::setOutput(QPlainTextEdit* textEdt) {
    this->textEdt = textEdt;
    outputWindow = vtkSmartPointer<CustomOutputWindow>::New();
    outputWindow->SetTextEdit(textEdt);
    vtkOutputWindow::SetInstance(outputWindow);
    vtkPythonInterpreter::SetRedirectOutput(true);
    vtkPythonInterpreter::Initialize();
}

void PythonModule::setOutputDirectory(const QString &outputDir) {
    QString command = QString("import os; os.chdir(r'%1')").arg(outputDir);
    vtkPythonInterpreter::RunSimpleString(command.toStdString().c_str());
}

vtkSmartPointer<vtkDoubleArray> PythonModule::getDoubleCube(const QString &name) {
    auto cube = getCube<vtkDoubleArray>(name, "vtkDoubleArray");
    cube->SetName(name.toStdString().c_str());
    return cube;
}

vtkSmartPointer<vtkIntArray> PythonModule::getIntCube(const QString &name) {
    auto cube = getCube<vtkIntArray>(name, "vtkIntArray");
    cube->SetName(name.toStdString().c_str());
    return cube;
}
