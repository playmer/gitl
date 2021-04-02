#include "Framework/MainWindow.hpp"
#include "Widgets/FileView.hpp"

namespace gitl
{
	FileView::FileView(MainWindow* aMainWindow)
		: Widget(aMainWindow)
		, mCommitsOnFileView(new QListView(this))
		, mTextEdit(new QPlainTextEdit(this))
	{

	}
}