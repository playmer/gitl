#include <QDateTime>

#include "Models/CommitModel.hpp"

namespace gitl {
    CommitModel::CommitModel(QObject* parent)
        : QAbstractTableModel{ parent }
    {
    }

    // Create a method to populate the model with data:
    void CommitModel::populateData(std::vector<CommitData>&& aCommits, QString&& aCommitSourceData)
    {
        mCommits = std::move(aCommits);
        mCommitSourceData = std::move(aCommitSourceData);
    }

    int CommitModel::rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return mCommits.size();
    }

    int CommitModel::columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return 5;
    }

    QVariant CommitModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || role != Qt::DisplayRole) {
            return QVariant();
        }

        switch (index.column())
        {
            case 0:  return "";//.bran mBranchOrTag[index.row()].join(";");
            case 1:  return "";//mCommits[index.row()];
            case 2:  
            {
                return mCommits[index.row()].Summary.toString();
            }
            case 3:
            {
                auto commit = mCommits[index.row()];
                return commit.AuthorName.toString() + "<" + commit.AuthorEmail.toString() + ">";
            }
            case 4: 
            {
                auto unixTime = mCommits[index.row()].AuthorDate.toString().toULongLong();
                return QDateTime::fromSecsSinceEpoch(unixTime).toString();
            }
            default: return QVariant();
        }
    }

    QVariant CommitModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section)
            {
                case 0:  return QString("Branches/Tags");
                case 1:  return QString("Graph");
                case 2:  return QString("Commit Message");
                case 3:  return QString("Author");
                case 4:  return QString("Time Committed");
                default: return QVariant();
            }
        }
        return QVariant();
    }

    
    std::optional<size_t> CommitModel::GetCommitIndex(QString commitId)
    {
        auto it = std::find_if(mCommits.begin(), mCommits.end(), [&commitId](const CommitData& aCommit)
        {
            return aCommit.Oid == commitId;
        });

        if (it != mCommits.end())
            return it - mCommits.begin();

        return std::nullopt;
    }

    CommitData& CommitModel::GetCommit(size_t index)
    {
        return mCommits[index];
    }
        
    const std::vector<CommitData>& CommitModel::GetCommits()
    {
        return mCommits;
    }
}