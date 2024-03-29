#pragma once

#include <variant>

#include <QAbstractTableModel>

#include "cppgit2/commit.hpp"
#include "cppgit2/repository.hpp"

namespace gitl
{
    class BranchModel;
    class CommitModel;

    class Git
    {
    public:
        Git(QString aInitialPath, QStringList aPathFilters);
        ~Git();
        
        void PopulateData(BranchModel& model);
        void PopulateData(CommitModel& model, cppgit2::reference& aReference);
        void PopulateData(CommitModel& model/*, cppgit2::commit& aCommit*/);
                
        QString& GetRepoPath()
        {
            return mRepoPath;
        }

    private:
        QString mRepoPath;
        QStringList mPathSpec;
    };
}