#include <iostream>
#include <exception>

#include <QApplication>
#include <QCommandLineParser>
#include <QFileDialog>

#include "GitHelper.hpp"

#include "Framework/MainWindow.hpp"
#include "Widgets/CommitView.hpp"

#include <QLineEdit>
#include <QStyledItemDelegate>


class CommitView;
class MainWindow;

class LimitTextDelegate : public QStyledItemDelegate
{
public:
    LimitTextDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        QLineEdit *editor = new QLineEdit(parent);
        int maxChar = index.row() + 1 ; // change this to suit you
        editor->setMaxLength(maxChar);
        return editor;
    }
};



int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("gitl");
    QApplication::setApplicationVersion("0.1");

    
    QCommandLineParser parser;
    parser.setApplicationDescription("gitl is a git history viewer, attempts to be gitk, but better.");
    parser.addHelpOption();
    parser.addVersionOption();
    
    QCommandLineOption pathOption(QStringList() << "p" << "path", QCoreApplication::translate("main", "Path of the git repository."), "path");
    parser.addOption(pathOption);
    
    QCommandLineOption filterOption(QStringList() << "f" << "filter", QCoreApplication::translate("main", "Paths to filter on the git repository."), "filter");
    parser.addOption(filterOption);
    
    parser.process(app);
    
    QStringList pathFilters;
    if (parser.isSet(filterOption))
        pathFilters = parser.values(filterOption);

    QString gitPath; 
    if (parser.isSet(pathOption))
        gitPath = parser.value(pathOption);
    else
        gitPath = QDir::currentPath();

    //auto gitPath = QFileDialog::getExistingDirectory(nullptr, "Open Git Repo", QDir::homePath());

    gitl::MainWindow window{gitPath, pathFilters};

    window.show();
    window.AddWidget<gitl::CommitView>("Commit", &window);
    
    return app.exec();
}


#include <Windows.h>

// https://stackoverflow.com/a/4023686
LPSTR* CommandLineToArgvA(LPSTR lpCmdLine, INT *pNumArgs)
{
    int retval;
    retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, NULL, 0);
    if (!SUCCEEDED(retval))
        return NULL;

    LPWSTR lpWideCharStr = (LPWSTR)malloc(retval * sizeof(WCHAR));
    if (lpWideCharStr == NULL)
        return NULL;

    retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, lpWideCharStr, retval);
    if (!SUCCEEDED(retval))
    {
        free(lpWideCharStr);
        return NULL;
    }

    int numArgs;
    LPWSTR* args;
    args = CommandLineToArgvW(lpWideCharStr, &numArgs);
    free(lpWideCharStr);
    if (args == NULL)
        return NULL;

    int storage = numArgs * sizeof(LPSTR);
    for (int i = 0; i < numArgs; ++ i)
    {
        BOOL lpUsedDefaultChar = FALSE;
        retval = WideCharToMultiByte(CP_ACP, 0, args[i], -1, NULL, 0, NULL, &lpUsedDefaultChar);
        if (!SUCCEEDED(retval))
        {
            LocalFree(args);
            return NULL;
        }

        storage += retval;
    }

    LPSTR* result = (LPSTR*)LocalAlloc(LMEM_FIXED, storage);
    if (result == NULL)
    {
        LocalFree(args);
        return NULL;
    }

    int bufLen = storage - numArgs * sizeof(LPSTR);
    LPSTR buffer = ((LPSTR)result) + numArgs * sizeof(LPSTR);
    for (int i = 0; i < numArgs; ++ i)
    {
        assert(bufLen > 0);
        BOOL lpUsedDefaultChar = FALSE;
        retval = WideCharToMultiByte(CP_ACP, 0, args[i], -1, buffer, bufLen, NULL, &lpUsedDefaultChar);
        if (!SUCCEEDED(retval))
        {
            LocalFree(result);
            LocalFree(args);
            return NULL;
        }

        result[i] = buffer;
        buffer += retval;
        bufLen -= retval;
    }

    LocalFree(args);

    *pNumArgs = numArgs;
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    LPSTR *argv;
    int argc;
 
     argv = CommandLineToArgvA(GetCommandLineA(), &argc);
     if (argv == NULL)
     {
         return 10;
     }

     main(argc, argv);
 
     LocalFree(argv);
     return 0;
}