#pragma once

#include <QComboBox>
#include <QMenuBar>
#include <QMainWindow>
#include <QTableView>

#include "ToolWindowManager.h"

#include "Models/CommitFilterProxyModel.hpp"

#include "Widgets/ClassTableFilter.hpp"

#include "GitHelper.hpp"


namespace gitl {
    class MainWindow : public QMainWindow
    {
    public:
        MainWindow(QString aGitPath, QStringList aPathFilters);

        ToolWindowManager* GetToolWindowManager();
    
        template <typename T, typename... Args>
        T* AddWidget(char const* aName, Args&&... args)
        {
            T* widget = new T(std::forward<Args>(args)...);

            widget->setWindowTitle(aName);

            auto area = mWindowManager->areaOf(mCommitTable);

            mWindowManager->addToolWindow(
                widget,
                ToolWindowManager::AreaReference{ToolWindowManager::AreaReferenceType::LeftWindowSide, area}, 
                widget->GetToolProperties());

            return widget;
        }

        QWidget* GetCommitTableWidget();
        QTableView* GetCommitTable();
        Git& GetGit();
        CommitModel* GetCommitModel();
        CommitFilterProxyModel* GetCommitFilteredModel();


    private:
        Git mGit;
        QMenuBar* mMenuBar;
        ToolWindowManager* mWindowManager;
        ClassTableFilter* mClassTableFilter;
        QWidget* mCommitTableWidget;
        QVBoxLayout* mCommitTableWidgetLayout;
        QComboBox* mBranchSelector;
        BranchModel* mBranchModel;
        QTableView* mCommitTable;
        CommitModel* mCommitModel;
        CommitFilterProxyModel* mCommitFilterModel;
    };
}