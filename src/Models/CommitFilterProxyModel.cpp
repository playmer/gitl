#include "Models/CommitFilterProxyModel.hpp"


namespace gitl {
    CommitFilterProxyModel::CommitFilterProxyModel(QObject* parent)
    {

    }

    QDateTime CommitFilterProxyModel::filterMinimumDate() const { return mMinDate; }
    void CommitFilterProxyModel::setFilterMinimumDate(QDateTime date)
    {
        mMinDate = date;
    }

    QDateTime CommitFilterProxyModel::filterMaximumDate() const { return mMaxDate; }
    void CommitFilterProxyModel::setFilterMaximumDate(QDateTime date)
    {
        mMaxDate = date;
    }

    QRegularExpression CommitFilterProxyModel::filterAuthor() const { return mAuthorRegex; }
    void CommitFilterProxyModel::setFilterAuthor(QString authorRegex)
    {
        mAuthorRegex = QRegularExpression(authorRegex, QRegularExpression::PatternOption::CaseInsensitiveOption);
    }

    QRegularExpression CommitFilterProxyModel::filterSummary() const { return mSummaryRegex; }
    void CommitFilterProxyModel::setFilterSummary(QString summaryRegex)
    {
        mSummaryRegex = QRegularExpression(summaryRegex, QRegularExpression::PatternOption::CaseInsensitiveOption);
    }

    void CommitFilterProxyModel::InvalidateFilter()
    {
        invalidateFilter();
    }

    void CommitFilterProxyModel::EnableFilter(bool aEnable)
    {
        mEnable = aEnable;
        invalidateFilter();
    }

    bool CommitFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
    {
        if (false == mEnable)
            return true;

        QModelIndex messageIndex = sourceModel()->index(sourceRow, 2, sourceParent);
        QModelIndex authorIndex = sourceModel()->index(sourceRow, 3, sourceParent);
        QModelIndex timeCommitedIndex = sourceModel()->index(sourceRow, 4, sourceParent);

        bool summary = sourceModel()->data(messageIndex).toString().contains(mSummaryRegex);
        bool author = sourceModel()->data(authorIndex).toString().contains(mAuthorRegex);
        bool date = dateInRange(QDateTime::fromString(sourceModel()->data(timeCommitedIndex).toString()));
        
        return summary && author && date;
    }

    bool CommitFilterProxyModel::dateInRange(QDateTime date) const
    {
        return (!mMinDate.isValid() || date > mMinDate)
                && (!mMaxDate.isValid() || date < mMaxDate);
    }
}