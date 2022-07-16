#include <QAbstractListModel>
#include <QModelIndex>
#include <QObject>
#pragma once

namespace gitl {
    enum class BranchOrTagType
    {
        RemoteBranch,
        LocalBranch,
        Tag
    };


    struct BranchOrTag
    {
        QString mName;
        BranchOrTagType mType;
    };

    class BranchModel : public QAbstractListModel
    {
    public:
        BranchModel(QObject *parent = 0);

        void populateData(std::vector<BranchOrTag>&& aBranchesOrTags);

        int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

        BranchOrTag& GetBranchOrTag(size_t index)
        {
            return mBranchesOrTags[index];
        }

    private:
        std::vector<BranchOrTag> mBranchesOrTags;
    };
}