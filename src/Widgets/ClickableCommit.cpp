#include "Framework/MainWindow.hpp"

#include "Models/CommitModel.hpp"

#include "Widgets/ClickableCommit.hpp"
#include "Widgets/CommitView.hpp"

namespace gitl {
    void ClickableCommit::mousePressEvent(QMouseEvent* event)
    {
        auto commitModel = mCommitView->GetMainWindow()->GetCommitModel();
        auto index =  commitModel->GetCommitIndex(mId);
    
        if (index.has_value())
        {
            auto sourceModelIndex = commitModel->index(index.value(), 0);
            auto filteredModel = mCommitView->GetMainWindow()->GetCommitFilteredModel();
        
            auto mappedIndex = filteredModel->mapFromSource(sourceModelIndex);

            if (false == mappedIndex.isValid())
            {
                // TODO: Uncheck the filter checkbox.
                filteredModel->EnableFilter(false);
                mappedIndex = filteredModel->mapFromSource(sourceModelIndex);
            }

            mCommitView->GetMainWindow()->GetCommitTable()->setCurrentIndex(mappedIndex);
            mCommitView->CommitSelectedIndex(index.value());
        }
    }
}