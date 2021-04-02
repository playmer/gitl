#pragma once

#include <QLabel>

#include "Widgets/CommitView.hpp"

namespace gitl {
    class ClickableCommit : public QLabel { 
    public:
        ClickableCommit(CommitView* parent, int aParentNum, const QString aId, Qt::WindowFlags f)
            : QLabel(nullptr) 
            , mCommitView{parent}
            , mId{aId}
        {
            setText(QString::asprintf("Parent %d: <u><b style=\"color:blue;\">%s</b></u>", aParentNum + 1, aId.toUtf8().toStdString().c_str()));
        }

        ~ClickableCommit() {}

        const QString& GetId()
        {
            return mId;
        }

    protected:
        void mousePressEvent(QMouseEvent* event);
        const QString mId;
        CommitView* mCommitView;
    };
}