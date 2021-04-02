#include <QGuiApplication>
#include <QScreen>

#include "Framework/MainWindow.hpp"

#include "Models/BranchModel.hpp"

namespace gitl {

        MainWindow::MainWindow(QString aGitPath, QStringList aPathFilters)
        : mGit{aGitPath, aPathFilters}
        , mMenuBar{ new QMenuBar{ this } }
        , mWindowManager{ new ToolWindowManager{ this } }
        {
            QMainWindow::setMenuBar(mMenuBar);

            setCentralWidget(mWindowManager);

        
            ///////////////
            // Models

            // Create model:
            mCommitModel = new CommitModel(this);
            //mCommitFilterModel = new QSortFilterProxyModel(this);
            mCommitFilterModel = new CommitFilterProxyModel(this);
        

            // Populate model with data:
            mGit.PopulateData(*mCommitModel);

            // Connect model to table view:
            mCommitFilterModel->setSourceModel(mCommitModel);


        
            //////////////////////////
            // Widgets
            mCommitTableWidget = new QWidget(this);
            mCommitTableWidget->setWindowTitle("Commit Table");
            mCommitTableWidgetLayout = new QVBoxLayout(mCommitTableWidget);

            ///////////////
            // Filtering
            mClassTableFilter = new ClassTableFilter(mCommitModel, mCommitFilterModel, this, this);

            ///////////////
            // Branch Selector
            mBranchSelector = new QComboBox(mCommitTableWidget);

            mBranchModel = new BranchModel(mBranchSelector);
            mGit.PopulateData(*mBranchModel);
            mBranchSelector->setModel(mBranchModel);
        
        
            ///////////////
            // Commit Table
            mCommitTable = new QTableView(mCommitTableWidget);
            mCommitTable->setSelectionBehavior(QAbstractItemView::SelectRows);
            mCommitTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

            mCommitTable->setModel(mCommitFilterModel);

            mCommitTable->setWindowTitle("Commit View");
            mCommitTable->resizeColumnsToContents();
            //mCommitTable->horizontalHeader()->setStretchLastSection(true);

        
            ///////////////
            // Layouting
            mCommitTableWidgetLayout->addWidget(mClassTableFilter);
            mCommitTableWidgetLayout->addWidget(mBranchSelector);
            mCommitTableWidgetLayout->addWidget(mCommitTable);
            mCommitTableWidget->setLayout(mCommitTableWidgetLayout);

            mWindowManager->addToolWindow(mCommitTableWidget, ToolWindowManager::EmptySpace);

            resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.7);
        }

        ToolWindowManager* MainWindow::GetToolWindowManager()
        {
            return mWindowManager;
        }

        QWidget* MainWindow::GetCommitTableWidget()
        {
            return mCommitTableWidget;
        }
    
        QTableView* MainWindow::GetCommitTable()
        {
            return mCommitTable;
        }

        Git& MainWindow::GetGit()
        {
            return mGit;
        }
    
        CommitModel* MainWindow::GetCommitModel()
        {
            return mCommitModel;
        }
    
    
        CommitFilterProxyModel* MainWindow::GetCommitFilteredModel()
        {
            return mCommitFilterModel;
        }
}