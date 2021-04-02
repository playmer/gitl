#pragma once

#include <QDateTime>
#include <QSortFilterProxyModel>

namespace gitl {
    class CommitFilterProxyModel : public QSortFilterProxyModel
    {
    public:
        CommitFilterProxyModel(QObject* parent = 0);

        QDateTime filterMinimumDate() const;
        void setFilterMinimumDate(QDateTime date);

        QDateTime filterMaximumDate() const;
        void setFilterMaximumDate(QDateTime date);

        QRegularExpression filterAuthor() const;
        void setFilterAuthor(QString authorRegex);

        QRegularExpression filterSummary() const;
        void setFilterSummary(QString summaryRegex);

        void InvalidateFilter();

        void EnableFilter(bool aEnable);

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

    private:
        bool dateInRange(QDateTime date) const;
    
        QRegularExpression mAuthorRegex;
        QRegularExpression mSummaryRegex;
        QDateTime mMinDate;
        QDateTime mMaxDate;

        bool mEnable;
    };
}