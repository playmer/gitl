#include <algorithm>

#include <QDir>
#include <QProcess>
#include <QObject>

#include "cppgit2/pathspec.hpp"

#include "GitHelper.hpp"

#include "Models/BranchModel.hpp"
#include "Models/CommitModel.hpp"

namespace gitl
{
    const QString cRequestStart = "START_OF_GITL_COMMIT_DATA_REQUEST:";
    constexpr size_t cRequestStartSize = sizeof("START_OF_GITL_COMMIT_DATA_REQUEST:");

    struct CommitDataIndexes
    {
        std::pair<size_t, size_t> Oid;
        std::pair<size_t, size_t> Parents;
        std::pair<size_t, size_t> AuthorName;
        std::pair<size_t, size_t> AuthorEmail;
        std::pair<size_t, size_t> AuthorDate;
        std::pair<size_t, size_t> CommitterName;
        std::pair<size_t, size_t> CommitterEmail;
        std::pair<size_t, size_t> CommitterDate;
        std::pair<size_t, size_t> Summary;
        std::pair<size_t, size_t> Body;
    };

    class GitCommitsProcess : public QObject
    {
    public:
        GitCommitsProcess(QString aRepoPath, QStringList aPathFilters)
        {
            mFullData.reserve(1024 * 1024 * 50);
            mIterator = QStringView(mFullData.begin(), mFullData.end());
            mProcess = new QProcess();
            
            this->connect(mProcess, &QProcess::readyReadStandardOutput, this, &GitCommitsProcess::readyReadStandardOutput);
            this->connect(mProcess, &QProcess::finished, this, &GitCommitsProcess::finished);

            QString program = "git";
            QStringList arguments;

            // TODO: Should confirm this format is correct and that we can't get extra separators in the summary.
            arguments << "log" << "--pretty=tformat:START_OF_GITL_COMMIT_DATA_REQUEST:%H\t%P\t%an\t%ae\t%at\t%cn\t%ce\t%ct\t%s\t\n%b" << "--";

            for (auto& path : aPathFilters)
                arguments << path;

            mProcess->setWorkingDirectory(aRepoPath);
            mProcess->start(program, arguments);
            
            if (!mProcess->waitForStarted())
                return ;
            
            mProcess->closeWriteChannel();
            mProcess->setReadChannel(QProcess::StandardOutput);
        }

        ~GitCommitsProcess()
        {
            delete mProcess;
        }

        void WaitForFinished()
        {
            if (!mProcess->waitForFinished())
                return;
            
            auto exitStatus = mProcess->exitStatus();
            auto exitCode = mProcess->exitCode();
        }

        void finished(int exitCode, QProcess::ExitStatus exitStatus)
        {
            while (-1 != mIterator.indexOf(cRequestStart))
                ParseCommit();
        }

        void readyReadStandardOutput()
        {
            // Reconstruct the iterator since we need to read data into the store.
            auto iteratorIndex = mIterator.begin() - mFullData.begin();
            mFullData += mProcess->readAllStandardOutput();
            mIterator = QStringView(mFullData.begin() + iteratorIndex, mFullData.end());

            while (mIterator.size() > cRequestStartSize && -1 != mIterator.indexOf(cRequestStart, cRequestStartSize))
                ParseCommit();
        }

        void ParseCommit()
        {
            const auto startOfCommit = mIterator.indexOf(cRequestStart);
            const auto endOfOid = mIterator.indexOf('\t', startOfCommit + cRequestStartSize);
            const auto endOfParents = mIterator.indexOf('\t', endOfOid + 1);
            const auto endOfAuthorName = mIterator.indexOf('\t', endOfParents + 1);
            const auto endOfAuthorEmail = mIterator.indexOf('\t', endOfAuthorName + 1);
            const auto endOfAuthorDate = mIterator.indexOf('\t', endOfAuthorEmail + 1);
            const auto endOfCommitterName = mIterator.indexOf('\t', endOfAuthorDate + 1);
            const auto endOfCommitterEmail = mIterator.indexOf('\t', endOfCommitterName + 1);
            const auto endOfCommitterDate = mIterator.indexOf('\t', endOfCommitterEmail + 1);
            const auto endOfSummary = mIterator.indexOf('\n', endOfCommitterDate + 1);
            auto endOfBody = mIterator.indexOf(cRequestStart, endOfCommitterDate + 1);

            if (-1 == endOfBody)
                endOfBody = mIterator.size();

            auto const iteratorBeginIndex = mIterator.begin() - mFullData.begin();
                        
            mCommitIndexes.emplace_back(
                std::pair<size_t, size_t>(iteratorBeginIndex + cRequestStartSize - 1  , iteratorBeginIndex + endOfOid),
                std::pair<size_t, size_t>(iteratorBeginIndex + 1 + endOfOid           , iteratorBeginIndex + endOfParents),
                std::pair<size_t, size_t>(iteratorBeginIndex + 1 + endOfParents       , iteratorBeginIndex + endOfAuthorName),
                std::pair<size_t, size_t>(iteratorBeginIndex + 1 + endOfAuthorName    , iteratorBeginIndex + endOfAuthorEmail),
                std::pair<size_t, size_t>(iteratorBeginIndex + 1 + endOfAuthorEmail   , iteratorBeginIndex + endOfAuthorDate),
                std::pair<size_t, size_t>(iteratorBeginIndex + 1 + endOfAuthorDate    , iteratorBeginIndex + endOfCommitterName),
                std::pair<size_t, size_t>(iteratorBeginIndex + 1 + endOfCommitterName , iteratorBeginIndex + endOfCommitterEmail),
                std::pair<size_t, size_t>(iteratorBeginIndex + 1 + endOfCommitterEmail, iteratorBeginIndex + endOfCommitterDate),
                std::pair<size_t, size_t>(iteratorBeginIndex + 1 + endOfCommitterDate , iteratorBeginIndex + endOfSummary - 1),
                std::pair<size_t, size_t>(iteratorBeginIndex + 1 + endOfSummary       , iteratorBeginIndex + endOfBody)
            );

            
            mIterator = QStringView(mIterator.begin() + endOfBody, mIterator.end());
        }
        
        QString mFullData;
        std::vector<CommitDataIndexes> mCommitIndexes;

    private:
        QProcess* mProcess;
        QStringView mIterator;
    };



    Git::Git(QString aInitialPath, QStringList aPathFilters)
        : mPathSpec{aPathFilters}
    {
        std::string initialPath = aInitialPath.toUtf8().toStdString();

        auto repoPath = cppgit2::repository::discover_path(initialPath);

        if (false == repoPath.HasResult())
            return;

        mRepoPath = repoPath.Result().c_str();
        
        std::vector<std::string> paths;
        for (auto& path : aPathFilters)
            paths.push_back(path.toUtf8().toStdString());
        
        // Need to transform the initial path to look like what we get back from
        // discover path before this works (something like): D:/My/Repo/.git/
        //if (initialPath != repoPath.Result())
        //    mPathSpec.push_back(initialPath);
    }
    
    void Git::PopulateData(BranchModel& model)
    {
        QString program = "git";
        QStringList arguments;

        arguments << "branch" << "-a";

        QProcess git;
        git.setWorkingDirectory(mRepoPath);
        git.start(program, arguments);
        
        if (!git.waitForStarted())
            return;

        git.closeWriteChannel();

        if (!git.waitForFinished())
            return;

        auto exitStatus = git.exitStatus();
        auto exitCode = git.exitCode();

        //QString result = QString(git.readAll());
        auto stderrResult = QString(git.readAllStandardError());
        auto stdoutResult = QString(git.readAllStandardOutput());

        auto iterator = QStringView(stdoutResult);

        std::vector<BranchOrTag> branchesOrTags;

        while (iterator.size())
        {
            auto endOfLine = iterator.indexOf('\n');
            auto branchName = QStringView(iterator.begin(), iterator.begin() + endOfLine);
            branchesOrTags.emplace_back(branchName.toString());
            iterator = QStringView(iterator.begin() + 1 + endOfLine, iterator.end());
        }

        model.populateData(std::move(branchesOrTags));
    }

    std::pair<std::vector<CommitData>, QString> GetCommitData(QString aInitialPath, QStringList aPathFilters)
    {
        using namespace cppgit2;

        GitCommitsProcess process(aInitialPath, aPathFilters);

        process.WaitForFinished();

        std::vector<CommitData> commitData;
        commitData.reserve(process.mCommitIndexes.size());
        
        for (auto& indexes : process.mCommitIndexes)
        {
            commitData.emplace_back(
                QStringView(process.mFullData.begin() + indexes.Oid.first, process.mFullData.begin() + indexes.Oid.second),
                QStringView(process.mFullData.begin() + indexes.Parents.first, process.mFullData.begin() + indexes.Parents.second),
                QStringView(process.mFullData.begin() + indexes.AuthorName.first, process.mFullData.begin() + indexes.AuthorName.second),
                QStringView(process.mFullData.begin() + indexes.AuthorEmail.first, process.mFullData.begin() + indexes.AuthorEmail.second),
                QStringView(process.mFullData.begin() + indexes.AuthorDate.first, process.mFullData.begin() + indexes.AuthorDate.second),
                QStringView(process.mFullData.begin() + indexes.CommitterName.first, process.mFullData.begin() + indexes.CommitterName.second),
                QStringView(process.mFullData.begin() + indexes.CommitterEmail.first, process.mFullData.begin() + indexes.CommitterEmail.second),
                QStringView(process.mFullData.begin() + indexes.CommitterDate.first, process.mFullData.begin() + indexes.CommitterDate.second),
                QStringView(process.mFullData.begin() + indexes.Summary.first, process.mFullData.begin() + indexes.Summary.second),
                QStringView(process.mFullData.begin() + indexes.Body.first, process.mFullData.begin() + indexes.Body.second)
            );
        }

        return std::pair<std::vector<CommitData>, QString>{std::move(commitData), std::move(process.mFullData)};
    }

    void Git::PopulateData(CommitModel& model/*, cppgit2::commit& aCommit*/)
    {
        auto commitData = GetCommitData(mRepoPath, mPathSpec);

        model.populateData(std::move(commitData.first), std::move(commitData.second));
    }

    void Git::PopulateData(CommitModel& model, cppgit2::reference& aBranchOrTagRef)
    {
        //mRepository.
    }

    Git::~Git()
    {

    }
}