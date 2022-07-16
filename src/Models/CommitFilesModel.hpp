#pragma once

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QObject>
#include <QVariant>

namespace gitl {
    struct FileDelta
    {
        enum class FileStatus
        {
            Modified,
            Added,
            Deleted,
            Renamed,
            Copied,
            Updated // (But Unmerged)
        };

        // The `id` is the `git_oid` of the item.
        FileStatus status;

        // `path` is relative to the working directory of the repository.
        QString path;
    };
    
    class CommitFilesModel : public QAbstractTableModel
    {
    public:
        CommitFilesModel(QObject *parent = 0);

        void populateData(QString& aRepoPath, QString& aCommitId);

        int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
        int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    private:
        std::vector<FileDelta> mFiles;
    };
}