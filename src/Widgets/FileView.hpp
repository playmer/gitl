#pragma once

#include <QListView>
#include <QPlainTextEdit>

#include "Framework/Widget.hpp"

namespace gitl
{
    class MainWindow;

    class FileView : public Widget<QWidget>
    {
    public:
        FileView(MainWindow* aMainWindow);

        void setFileAndCommit(QString aFile, QString aCommit);

    private:
        QListView* mCommitsOnFileView;
        QPlainTextEdit* mTextEdit;
        CommitModel* mFileCommitModel;
    };
}