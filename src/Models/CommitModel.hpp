#pragma once

#include <QAbstractTableModel>
#include <QObject>
#include <QModelIndex>

namespace gitl {
    struct CommitData
    {
        QStringView Oid;
        QStringView Parents;
        QStringView AuthorName;
        QStringView AuthorEmail;
        QStringView AuthorDate;
        QStringView CommitterName;
        QStringView CommitterEmail;
        QStringView CommitterDate;
        QStringView Summary;
        QStringView Body;
    };

    class CommitModel : public QAbstractTableModel
    {
    public:
        CommitModel(QObject *parent = 0);

        void populateData(std::vector<CommitData>&& aCommits, QString&& aCommitSourceData);

        int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
        int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
        
        std::optional<size_t> GetCommitIndex(QString aId);
        CommitData& GetCommit(size_t index);
        const std::vector<CommitData>& GetCommits();

    private:
        std::vector<CommitData> mCommits;
        QString mCommitSourceData;
    };
}