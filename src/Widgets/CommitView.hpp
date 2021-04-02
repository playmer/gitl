#pragma once

#include <QHeaderView>
#include <QLineEdit>
#include <QSplitter>
#include <QTableView>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "Framework/Widget.hpp"
#include "Framework/MainWindow.hpp"

namespace gitl {
    class ClickableCommit;
    class CommitFilesModel;

    class CommitView : public Widget<QWidget>
    {
    public:
        CommitView(MainWindow* aMainWindow);
        ~CommitView();

        ToolWindowManager::AreaReference GetToolArea() override;
        void CommitSelected(const QModelIndex& aIndex);
        void CommitSelectedIndex(size_t aIndex);

    

    private:
        QWidget* mInfoWidget;
        QVBoxLayout* mInfoWidgetLayout;
        QLabel* mAuthorLabel;
        QLabel* mCommitterLabel;
        std::vector<ClickableCommit*> mParentCommits;
    
        QWidget* mCommitMessage;
        QVBoxLayout* mCommitMessageLayout;
        QLineEdit* mSummary;
        QTextEdit* mFullCommitMessage;


        QTableView* mFiles;
        CommitFilesModel* mCommitFilesModel = nullptr;
        QVBoxLayout* mLayout;
        QSplitter* mSplitter;
        //QWidget *mSubWidget;
        //QVBoxLayout *mSubWidgetLayout;
        //QScrollArea *mScrollArea;

        void ConstructSubWidgets();
    };
}