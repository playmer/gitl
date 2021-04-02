#include <variant>

#include "Models/BranchModel.hpp"

namespace gitl {
    BranchModel::BranchModel(QObject *parent) : QAbstractListModel(parent)
    {
    }
    
    void BranchModel::populateData(std::vector<BranchOrTag>&& aBranchesOrTags)
    {
        mBranchesOrTags = std::move(aBranchesOrTags);
    }

    int BranchModel::rowCount(const QModelIndex& parent) const
    {
        return mBranchesOrTags.size();
    }

    QVariant BranchModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid() || role != Qt::DisplayRole) {
            return QVariant();
        }

        auto& branchOrTag = mBranchesOrTags[index.row()];

        if (branchOrTag.mType == BranchOrTagType::LocalBranch || branchOrTag.mType == BranchOrTagType::RemoteBranch) // branch
            return branchOrTag.mName;
        else if (branchOrTag.mType == BranchOrTagType::Tag)
            return branchOrTag.mName;
        else
            return QVariant();
    }
}