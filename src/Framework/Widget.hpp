#pragma once

#include "ToolWindowManager.h"

#include "Framework/MainWindow.hpp"

namespace gitl {
    // tType must derive from QWidget
    template <typename tInheritingType = QWidget>
    class Widget : public tInheritingType
    {
    public:
        Widget(MainWindow* aMainWindow)
            : tInheritingType{}
            , mMainWindow{ aMainWindow }
        {

        }

        virtual ToolWindowManager::AreaReference GetToolArea() = 0;

        virtual ToolWindowManager::ToolWindowProperty GetToolProperties()
        {
            return ToolWindowManager::ToolWindowProperty(0);
        };

        MainWindow* GetMainWindow()
        {
            return mMainWindow;
        }

    protected:
        MainWindow* mMainWindow;

    private:
    //    friend class Workspace;
    };
}