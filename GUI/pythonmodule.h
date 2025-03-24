#ifndef PYTHONMODULE_H
#define PYTHONMODULE_H

// Проверка и удаление потенциально конфликтующих макросов
#ifdef _POSIX_C_SOURCE
#  undef _POSIX_C_SOURCE
#endif

#ifdef _XOPEN_SOURCE
#  undef _XOPEN_SOURCE
#endif

// Убираем макросы Qt, которые конфликтуют с заголовками Python
#ifdef slots
#undef slots
#define PYTHONQT_RESTORE_KEYWORDS
#endif

// Специальные правки для совместимости с MinGW
#if defined(__MINGW32__)
#  include <cmath>
#  include <math.h>
#endif

// Настройка для использования релизной версии Python, если включен флаг PYTHONQT_USE_RELEASE_PYTHON_FALLBACK
#if defined(PYTHONQT_USE_RELEASE_PYTHON_FALLBACK) && defined(_DEBUG)
#undef _DEBUG
#if defined(_MSC_VER) && _MSC_VER >= 1400
#define _CRT_NOFORCE_MANIFEST 1
#define _STL_NOFORCE_MANIFEST 1
#endif
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

// Убираем макрос truncate, если он был определён (актуально для Linux)
#ifdef truncate
# undef truncate
#endif

// Восстанавливаем макрос slots из Qt, если он был изменён
#ifdef PYTHONQT_RESTORE_KEYWORDS
#define slots Q_SLOTS
#undef PYTHONQT_RESTORE_KEYWORDS
#endif

// Проверка версии Python и добавление соответствующих макросов для упрощения кода
#if PY_MAJOR_VERSION >= 3
#define PY3K
#define PyString_FromString PyUnicode_FromString
#define PyString_AS_STRING  PyUnicode_AsUTF8
#define PyString_AsString   PyUnicode_AsUTF8
#define PyString_FromFormat PyUnicode_FromFormat
#define PyString_Check      PyUnicode_Check

#define PyInt_Type     PyLong_Type
#define PyInt_FromLong PyLong_FromLong
#define PyInt_AS_LONG  PyLong_AS_LONG
#define PyInt_Check    PyLong_Check
#define PyInt_AsLong   PyLong_AsLong

#else
#define PyBytes_Type      PyString_Type
#define PyBytes_Check     PyString_Check
#define PyBytes_AS_STRING PyString_AS_STRING
#define PyBytes_AsString  PyString_AsString
#define PyBytes_GET_SIZE  PyString_GET_SIZE
#define PyBytes_FromStringAndSize PyString_FromStringAndSize
#endif

#include <vtkPython.h>
#include <vtkPythonUtil.h>
#include <vtkPythonInterpreter.h>
#include <QPlainTextEdit>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>

#include "customoutputwindow.h"

class PythonModule {
public:
    PythonModule();
    void run(const QString& script);
    void setOutput(QPlainTextEdit* textEdt);
    void setOutputDirectory(const QString& outputDir);
    vtkSmartPointer<vtkDoubleArray> getDoubleCube(const QString& name);
    vtkSmartPointer<vtkIntArray> getIntCube(const QString& name);
private:
    PyObject* mainModule;
    PyObject* globalDict;
    QPlainTextEdit* textEdt;
    vtkSmartPointer<CustomOutputWindow> outputWindow;

    template <typename T>
    vtkSmartPointer<T> getCube(const QString& name, const char* typeName);
};

template <typename T>
vtkSmartPointer<T> PythonModule::getCube(const QString& name, const char* typeName) {
    PyObject* vtkArrayObject = PyDict_GetItemString(globalDict, name.toStdString().c_str());
    vtkSmartPointer<T> newArray = vtkSmartPointer<T>::New();
    newArray->SetName(name.toStdString().c_str());
    if (vtkArrayObject) {
        T* vtkArray = T::SafeDownCast(vtkPythonUtil::GetPointerFromObject(vtkArrayObject, typeName));
        if (vtkArray) {
            newArray->DeepCopy(vtkArray);
        } else {
            textEdt->appendPlainText(QString("Не удалось преобразовать объект %1 в %2").arg(name).arg(typeName));
        }
    } else {
        textEdt->appendPlainText(QString("Объект с именем %1 не найден в глобальном словаре Python").arg(name));
    }
    return newArray;
}

#endif // PYTHONMODULE_H
