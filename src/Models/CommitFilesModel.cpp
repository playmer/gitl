#include <QProcess>

#include "process.hpp"

#include "Models/CommitFilesModel.hpp"

namespace gitl {
    CommitFilesModel::CommitFilesModel(QObject *parent) : QAbstractTableModel(parent)
    {
    }

    // Create a method to populate the model with data:
    void CommitFilesModel::populateData(QString& aRepoPath, QString& aCommitId)
    {
        mFiles.clear();

        
        std::string data;

        std::string command = "git show --oneline --raw --name-status ";
        command += aCommitId.toUtf8().toStdString();
        command += " --";

        TinyProcessLib::Process process1a(command, aRepoPath.toUtf8().toStdString(), [&data](const char *bytes, size_t n) {
            data += std::string(bytes, n);
        });
        auto exit_status=process1a.get_exit_status();

        auto stdoutResult = QString::fromStdString(data);

        //QString program = "git";
        //QStringList arguments;
        //
        // TODO: Should confirm this format is correct and that we can't get extra separators in the summary.
        //arguments << "diff-tree" << "--no-commit-id" << "--name-status" << "-r" <<  aCommitId << "--";
        //
        //QProcess git;
        //git.setWorkingDirectory(aRepoPath);
        //git.start(program, arguments);
        //
        //if (!git.waitForStarted())
        //    return;
        //
        //git.closeWriteChannel();
        //
        //if (!git.waitForFinished())
        //    return;
        //
        //auto exitStatus = git.exitStatus();
        //auto exitCode = git.exitCode();
        //
        ////QString result = QString(git.readAll());
        //auto stderrResult = QString(git.readAllStandardError());
        //auto stdoutResult = QString(git.readAllStandardOutput());

        auto iterator = QStringView(stdoutResult);
        
        // skip first line
        auto endOfLine = iterator.indexOf('\n');
        iterator = QStringView(iterator.begin() + 1 + endOfLine, iterator.end());

        while (iterator.size())
        {
            auto endOfStatus = iterator.indexOf('\t');
            auto endOfLine = iterator.indexOf('\n');
            QStringView status{ iterator.begin(), endOfStatus };
            QStringView file{ iterator.begin() + endOfStatus + 1, iterator.begin() + endOfLine };

            iterator = QStringView(iterator.begin() + 1 + endOfLine, iterator.end());

            FileDelta::FileStatus fileStatus;

            if (status.startsWith('M'))
                fileStatus = FileDelta::FileStatus::Modified;
            else if (status.startsWith('A'))
                fileStatus = FileDelta::FileStatus::Added;
            else if (status.startsWith('D'))
                fileStatus = FileDelta::FileStatus::Deleted;
            else if (status.startsWith('R'))
                fileStatus = FileDelta::FileStatus::Renamed;
            else if (status.startsWith('C'))
                fileStatus = FileDelta::FileStatus::Copied;
            else if (status.startsWith('U'))
                fileStatus = FileDelta::FileStatus::Updated;

            mFiles.emplace_back(fileStatus, file.toString());
        }
    }

    int CommitFilesModel::rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return mFiles.size();
    }

    int CommitFilesModel::columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return 2;
    }

    QVariant CommitFilesModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || role != Qt::DisplayRole) {
            return QVariant();
        }

        switch (index.column())
        {
            case 0:  
            {
                switch (mFiles[index.row()].status)
                {
                    case FileDelta::FileStatus::Added: return QString("Added");
                    case FileDelta::FileStatus::Deleted: return QString("Deleted");
                    case FileDelta::FileStatus::Modified: return QString("Modified");
                    case FileDelta::FileStatus::Renamed: return QString("Renamed");
                    case FileDelta::FileStatus::Copied: return QString("Copied");
                    case FileDelta::FileStatus::Updated: return QString("Untracked");
                    default: return QVariant();
                }
            }
            case 1: return mFiles[index.row()].path;
            default: return QVariant();
        }
    }

    QVariant CommitFilesModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section)
            {
                case 0:  return QString("Status");
                case 1:  return QString("File Name");
                default: return QVariant();
            }
        }
        return QVariant();
    }

}