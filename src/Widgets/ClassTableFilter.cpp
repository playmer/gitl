#include "QDateTimeEdit"
#include "QLabel"
#include "QVBoxLayout"

#include "Framework/MainWindow.hpp"

#include "Widgets/ClassTableFilter.hpp"


namespace gitl {
    ClassTableFilter::ClassTableFilter(CommitModel* aCommitModel, CommitFilterProxyModel* aCommitProxyModel, MainWindow* aMainWindow, QWidget* parent) : QWidget(parent)
    {
        mCommitProxyModel = aCommitProxyModel;
        QDateTime maxDate;
        QDateTime minDate;

        if (aCommitModel->GetCommits().size() > 0)
        {
            
            auto latestCommit = aCommitModel->GetCommit(0);;
            auto oldestCommit = aCommitModel->GetCommit(aCommitModel->GetCommits().size() - 1);;
            
            auto latestUnixTime = latestCommit.AuthorDate.toString().toULongLong();
            auto oldestUnixTime = oldestCommit.AuthorDate.toString().toULongLong();
            maxDate = QDateTime::fromSecsSinceEpoch(latestUnixTime).toLocalTime().addDays(1);
            minDate = QDateTime::fromSecsSinceEpoch(oldestUnixTime).toLocalTime().addDays(-1);
        }

        mFilteringWidgetLayout = new QVBoxLayout(this);
        mFilteringSubWidget = new QWidget(this);
        mFilteringSubWidgetLayout = new QVBoxLayout(mFilteringSubWidget);
        
        // Filtering Enabled
        mFilteringEnabledLabel = new QLabel(this);
        mFilteringEnabledLabel->setText("Filtering:");
        mFilteringEnabledCheckBox = new QCheckBox(this);
        mFilteringEnabledCheckBox->setTristate(false);
        mFilteringEnabledLayout = new QHBoxLayout();
        mFilteringEnabledLayout->addWidget(mFilteringEnabledLabel);
        mFilteringEnabledLayout->addWidget(mFilteringEnabledCheckBox);
        
        this->connect(mFilteringEnabledCheckBox, &QCheckBox::stateChanged, this, &ClassTableFilter::CheckBoxChanged);
        
        // Author
        mFilteringAuthorLabel = new QLabel(mFilteringSubWidget);
        mFilteringAuthorLabel->setText("Author:");
        mFilteringAuthorLineEdit = new QLineEdit(mFilteringSubWidget);
        mFilteringAuthorLayout = new QHBoxLayout();
        mFilteringAuthorLayout->addWidget(mFilteringAuthorLabel);
        mFilteringAuthorLayout->addWidget(mFilteringAuthorLineEdit);
        
        //this->connect(mFilteringAuthorLineEdit, &QLineEdit::editingFinished, this, &ClassTableFilter::AuthorChanged);

        // Summary
        mFilteringSummaryLabel = new QLabel(mFilteringSubWidget);
        mFilteringSummaryLabel->setText("Summary:");
        mFilteringSummaryLineEdit = new QLineEdit(mFilteringSubWidget);
        mFilteringSummaryLayout = new QHBoxLayout();
        mFilteringSummaryLayout->addWidget(mFilteringSummaryLabel);
        mFilteringSummaryLayout->addWidget(mFilteringSummaryLineEdit);
        
        //this->connect(mFilteringSummaryLineEdit, &QLineEdit::editingFinished, this, &ClassTableFilter::SummaryChanged);
        
        // Minimum Date
        mFilteringMinimumDateLabel = new QLabel(mFilteringSubWidget);
        mFilteringMinimumDateLabel->setText("Minimum Date:");
        mFilteringMinimumDateEdit = new QDateTimeEdit(mFilteringSubWidget);
        mFilteringMinimumDateEdit->setDateTime(minDate);
        mFilteringMinimumDateEdit->setCalendarPopup(true);
        mFilteringMinimumDateLayout = new QHBoxLayout();
        mFilteringMinimumDateLayout->addWidget(mFilteringMinimumDateLabel);
        mFilteringMinimumDateLayout->addWidget(mFilteringMinimumDateEdit);
        
        //this->connect(mFilteringMinimumDateEdit, &QDateTimeEdit::dateTimeChanged, this, &ClassTableFilter::MinDateChanged);

        // Maximum Date
        mFilteringMaximumDateLabel = new QLabel(mFilteringSubWidget);
        mFilteringMaximumDateLabel->setText("Maximum Date:");
        mFilteringMaximumDateEdit = new QDateTimeEdit(mFilteringSubWidget);
        mFilteringMaximumDateEdit->setDateTime(maxDate);
        mFilteringMaximumDateEdit->setCalendarPopup(true);
        mFilteringMaximumDateLayout = new QHBoxLayout();
        mFilteringMaximumDateLayout->addWidget(mFilteringMaximumDateLabel);
        mFilteringMaximumDateLayout->addWidget(mFilteringMaximumDateEdit);
        
        //this->connect(mFilteringMaximumDateEdit, &QDateTimeEdit::dateTimeChanged, this, &ClassTableFilter::MaxDateChanged);

        mSetFilterButton = new QPushButton(mFilteringSubWidget);
        mSetFilterButton->setText("Set Filter");
        this->connect(mSetFilterButton, &QPushButton::clicked, this, &ClassTableFilter::FilterSet);
        
        // Layouting
        mFilteringSubWidgetLayout->addLayout(mFilteringAuthorLayout);
        mFilteringSubWidgetLayout->addLayout(mFilteringSummaryLayout);
        mFilteringSubWidgetLayout->addLayout(mFilteringMinimumDateLayout);
        mFilteringSubWidgetLayout->addLayout(mFilteringMaximumDateLayout);
        mFilteringSubWidgetLayout->addWidget(mSetFilterButton);

        mFilteringSubWidget->setLayout(mFilteringSubWidgetLayout);
        mFilteringWidgetLayout->addLayout(mFilteringEnabledLayout);
        mFilteringWidgetLayout->addWidget(mFilteringSubWidget);
        this->setLayout(mFilteringWidgetLayout);
        mFilteringSubWidget->hide();
    }

    void ClassTableFilter::CheckBoxChanged(int i)
    {
        if (i == Qt::Unchecked)
        {
            mCommitProxyModel->EnableFilter(false);
            mFilteringSubWidget->hide();
        }
        else
        {
            mCommitProxyModel->EnableFilter(true);
            mFilteringSubWidget->show();
        }
    }

    void ClassTableFilter::FilterSet(bool checked)
    {
        mCommitProxyModel->setFilterAuthor(mFilteringAuthorLineEdit->text());
        mCommitProxyModel->setFilterSummary(mFilteringSummaryLineEdit->text());
        mCommitProxyModel->setFilterMinimumDate(mFilteringMinimumDateEdit->dateTime());
        mCommitProxyModel->setFilterMaximumDate(mFilteringMaximumDateEdit->dateTime());
        mCommitProxyModel->InvalidateFilter();
    }
}