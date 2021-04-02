#pragma once

#include <QCheckbox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include "Models/CommitFilterProxyModel.hpp"
#include "Models/CommitModel.hpp"

namespace gitl {
    class MainWindow;

    class ClassTableFilter : public QWidget
    {
    public:
        ClassTableFilter(CommitModel* aCommitModel, CommitFilterProxyModel* aCommitProxyModel, MainWindow* aMainWindow, QWidget* parent = nullptr);

        void CheckBoxChanged(int i);
        void FilterSet(bool checked);

    private:
        CommitFilterProxyModel* mCommitProxyModel = nullptr;

        QVBoxLayout* mFilteringWidgetLayout = nullptr;
        QWidget* mFilteringSubWidget = nullptr;
        QVBoxLayout* mFilteringSubWidgetLayout = nullptr;

        // Filtering Enabled
        QLabel* mFilteringEnabledLabel = nullptr;
        QCheckBox* mFilteringEnabledCheckBox = nullptr;
        QHBoxLayout* mFilteringEnabledLayout = nullptr;
    
        // Author
        QLabel* mFilteringAuthorLabel = nullptr;
        QLineEdit* mFilteringAuthorLineEdit = nullptr;
        QHBoxLayout* mFilteringAuthorLayout = nullptr;

        // Summary
        QLabel* mFilteringSummaryLabel = nullptr;
        QLineEdit* mFilteringSummaryLineEdit = nullptr;
        QHBoxLayout* mFilteringSummaryLayout = nullptr;

        // Min Date
        QLabel* mFilteringMinimumDateLabel = nullptr;
        QDateTimeEdit* mFilteringMinimumDateEdit = nullptr;
        QHBoxLayout* mFilteringMinimumDateLayout = nullptr;

        // Max Date
        QLabel* mFilteringMaximumDateLabel = nullptr;
        QDateTimeEdit* mFilteringMaximumDateEdit = nullptr;
        QHBoxLayout* mFilteringMaximumDateLayout = nullptr;

        // Set Filter Button
        QPushButton* mSetFilterButton = nullptr;
    };
}