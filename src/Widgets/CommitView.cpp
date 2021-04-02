#include "Models/CommitFilesModel.hpp"

#include "Widgets/CommitView.hpp"
#include "Widgets/ClickableCommit.hpp"

namespace gitl {
    CommitView::CommitView(MainWindow* aMainWindow)
        : Widget{ aMainWindow }
    {
        this->connect(mMainWindow->GetCommitTable(), &QTableView::doubleClicked, this, &CommitView::CommitSelected);
        
        mLayout = new QVBoxLayout(this);
        mSplitter = new QSplitter(this);
        mSplitter->setOrientation(Qt::Orientation::Vertical);
        mLayout->addWidget(mSplitter);
        
        //////////////////////
        // File Table Widget
        mInfoWidget = new QWidget();
        
        mInfoWidgetLayout = new QVBoxLayout(static_cast<QWidget*>(mInfoWidget));
        
        /////////////
        // CommitMessage
        mCommitMessage = new QWidget(mInfoWidget);
        mCommitMessageLayout = new QVBoxLayout(mCommitMessage);
        mSummary = new QLineEdit(mCommitMessage);
        mSummary->setReadOnly(true);
        mCommitMessageLayout->addWidget(mSummary);

        mFullCommitMessage = new QTextEdit(mCommitMessage);
        mFullCommitMessage->setReadOnly(true);
        mCommitMessageLayout->addWidget(mFullCommitMessage);

        mCommitMessage->setLayout(mCommitMessageLayout);
        mInfoWidgetLayout->addWidget(mCommitMessage);
        
        /////////////
        // Authors
        mAuthorLabel = new QLabel(mInfoWidget);
        mInfoWidgetLayout->addWidget(mAuthorLabel);
        mCommitterLabel = new QLabel(mInfoWidget);
        mInfoWidgetLayout->addWidget(mCommitterLabel);
        
        // Commits
        //mParentCommits;

        mInfoWidget->setLayout(mInfoWidgetLayout);

        mSplitter->addWidget(mInfoWidget);

        //////////////////////
        // File Table Widget
        mFiles = new QTableView();
        mFiles->setSelectionBehavior(QAbstractItemView::SelectRows);
        mFiles->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        mFiles->setWindowTitle("Changed Files");
        mFiles->horizontalHeader()->setStretchLastSection(true);

        mSplitter->addWidget(mFiles);
        
        //mLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    }

    CommitView::~CommitView()
    {

    }

    ToolWindowManager::AreaReference CommitView::GetToolArea()
    {
        auto area = mMainWindow->GetToolWindowManager()->areaOf(mMainWindow->GetCommitTableWidget());

        if (area != nullptr)
        {
            return ToolWindowManager::AreaReference{
            ToolWindowManager::AreaReferenceType::RightOf,
            area
            };
        }
        else
        {
            return ToolWindowManager::AreaReference{
            ToolWindowManager::AreaReferenceType::LastUsedArea
            };
        }
    }

    
    void CommitView::CommitSelected(const QModelIndex& aIndex)
    {
        if (!aIndex.isValid())
        {
            return;
        }
        
        CommitSelectedIndex(mMainWindow->GetCommitFilteredModel()->mapToSource(aIndex).row());
    }
        
    void CommitView::CommitSelectedIndex(size_t aIndex)
    {
        auto commit = mMainWindow->GetCommitModel()->GetCommit(aIndex);
        QString commitId(commit.Oid.toString());

        mSummary->setText(commit.Summary.toString());
        mFullCommitMessage->setText(commit.Body.toString());
        
        /////////////
        // Parents, must be done dynamically
        for (auto& widget : mParentCommits)
        {
            mInfoWidgetLayout->removeWidget(widget);
            delete widget;
        }
        mParentCommits.clear();

        size_t i = 0;
        for (auto& parent : commit.Parents.split(QRegularExpression(" "), Qt::SkipEmptyParts))
        {
            auto clickableLabel = new ClickableCommit(this, i, parent.toString(), Qt::WindowType::Widget);
            mParentCommits.emplace_back(clickableLabel);
            mInfoWidgetLayout->addWidget(clickableLabel);

            ++i;
        }
        
        mAuthorLabel->setText(QString("Author: ") + commit.AuthorName.toString() + "<" + commit.AuthorEmail.toString() + ">");
        
        if (commit.AuthorEmail == commit.CommitterEmail)
            mCommitterLabel->hide();
        else
            mCommitterLabel->show();
        
        mCommitterLabel->setText(QString("Author: ") + commit.CommitterName.toString() + "<" + commit.CommitterEmail.toString() + ">");

        mCommitFilesModel = new CommitFilesModel(this);
        mCommitFilesModel->populateData(mMainWindow->GetGit().GetRepoPath(), commitId);
        mFiles->setModel(mCommitFilesModel);
    }
}