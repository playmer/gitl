#include <algorithm>

#include <QDir>
#include <QProcess>
#include <QObject>

#include "ProcessHelper.hpp"

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

    /*
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
    */


    Git::Git(QString aInitialPath, QStringList aPathFilters)
        : mPathSpec{aPathFilters}
    {
        //std::wstring initialPath = aInitialPath.toStdWString();
        std::string initialPath = aInitialPath.toUtf8().toStdString();

        std::string repoPath;

        TinyProcessLib::Process process1a("git rev-parse --show-toplevel", initialPath, [&repoPath](const char *bytes, size_t n) {
            repoPath = std::string(bytes, n);
        });
        auto exit_status=process1a.get_exit_status();

        //auto repoPath = cppgit2::repository::discover_path(initialPath);

        //if (false == repoPath.HasResult())
        //    return;
        auto index = repoPath.find("\n");

        if (index != std::string::npos)
        {
            repoPath.erase(index, 1);
        }

        mRepoPath = repoPath.c_str();
        
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
        
        QString fullData;
        QStringView iterator;
        
        fullData.reserve(1024 * 1024 * 50);
        iterator = QStringView(fullData.begin(), fullData.end());
        std::vector<CommitDataIndexes> commitIndexes;

        std::string command = "git log \"--pretty=tformat:START_OF_GITL_COMMIT_DATA_REQUEST:%H\t%P\t%an\t%ae\t%at\t%cn\t%ce\t%ct\t%s\t\n%b\" --";

        auto tryParse = [&commitIndexes, &fullData, &iterator]()
        {
            const auto startOfCommit = iterator.indexOf(cRequestStart);
            const auto endOfOid = iterator.indexOf('\t', startOfCommit + cRequestStartSize);
            const auto endOfParents = iterator.indexOf('\t', endOfOid + 1);
            const auto endOfAuthorName = iterator.indexOf('\t', endOfParents + 1);
            const auto endOfAuthorEmail = iterator.indexOf('\t', endOfAuthorName + 1);
            const auto endOfAuthorDate = iterator.indexOf('\t', endOfAuthorEmail + 1);
            const auto endOfCommitterName = iterator.indexOf('\t', endOfAuthorDate + 1);
            const auto endOfCommitterEmail = iterator.indexOf('\t', endOfCommitterName + 1);
            const auto endOfCommitterDate = iterator.indexOf('\t', endOfCommitterEmail + 1);
            const auto endOfSummary = iterator.indexOf('\n', endOfCommitterDate + 1);
            auto endOfBody = iterator.indexOf(cRequestStart, endOfCommitterDate + 1);

            if (-1 == endOfBody)
                endOfBody = iterator.size();

            auto const iteratorBeginIndex = iterator.begin() - fullData.begin();
                        
            commitIndexes.emplace_back(
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

            
            iterator = QStringView(iterator.begin() + endOfBody, iterator.end());
        };

        TinyProcessLib::Process process1a(command, aInitialPath.toUtf8().toStdString(), [&fullData, &iterator, &tryParse](const char *bytes, size_t n) {
            // Reconstruct the iterator since we need to read data into the store.
            auto iteratorIndex = iterator.begin() - fullData.begin();
            auto view = QString::fromUtf8(bytes, n);
            fullData.append(view);
            iterator = QStringView(fullData.begin() + iteratorIndex, fullData.end());

            while (iterator.size() > cRequestStartSize && -1 != iterator.indexOf(cRequestStart, cRequestStartSize))
                tryParse();

        });
        auto exit_status=process1a.get_exit_status();
        
        std::vector<CommitData> commitData;
        
        for (auto& indexes : commitIndexes)
        {
            commitData.emplace_back(
                QStringView(fullData.begin() + indexes.Oid.first, fullData.begin() + indexes.Oid.second),
                QStringView(fullData.begin() + indexes.Parents.first, fullData.begin() + indexes.Parents.second),
                QStringView(fullData.begin() + indexes.AuthorName.first, fullData.begin() + indexes.AuthorName.second),
                QStringView(fullData.begin() + indexes.AuthorEmail.first, fullData.begin() + indexes.AuthorEmail.second),
                QStringView(fullData.begin() + indexes.AuthorDate.first, fullData.begin() + indexes.AuthorDate.second),
                QStringView(fullData.begin() + indexes.CommitterName.first, fullData.begin() + indexes.CommitterName.second),
                QStringView(fullData.begin() + indexes.CommitterEmail.first, fullData.begin() + indexes.CommitterEmail.second),
                QStringView(fullData.begin() + indexes.CommitterDate.first, fullData.begin() + indexes.CommitterDate.second),
                QStringView(fullData.begin() + indexes.Summary.first, fullData.begin() + indexes.Summary.second),
                QStringView(fullData.begin() + indexes.Body.first, fullData.begin() + indexes.Body.second)
            );
        }


        //GitCommitsProcess process(aInitialPath, aPathFilters);
        //
        //process.WaitForFinished();
        //
        //std::vector<CommitData> commitData;
        //commitData.reserve(process.mCommitIndexes.size());
        //
        //for (auto& indexes : process.mCommitIndexes)
        //{
        //    commitData.emplace_back(
        //        QStringView(process.mFullData.begin() + indexes.Oid.first, process.mFullData.begin() + indexes.Oid.second),
        //        QStringView(process.mFullData.begin() + indexes.Parents.first, process.mFullData.begin() + indexes.Parents.second),
        //        QStringView(process.mFullData.begin() + indexes.AuthorName.first, process.mFullData.begin() + indexes.AuthorName.second),
        //        QStringView(process.mFullData.begin() + indexes.AuthorEmail.first, process.mFullData.begin() + indexes.AuthorEmail.second),
        //        QStringView(process.mFullData.begin() + indexes.AuthorDate.first, process.mFullData.begin() + indexes.AuthorDate.second),
        //        QStringView(process.mFullData.begin() + indexes.CommitterName.first, process.mFullData.begin() + indexes.CommitterName.second),
        //        QStringView(process.mFullData.begin() + indexes.CommitterEmail.first, process.mFullData.begin() + indexes.CommitterEmail.second),
        //        QStringView(process.mFullData.begin() + indexes.CommitterDate.first, process.mFullData.begin() + indexes.CommitterDate.second),
        //        QStringView(process.mFullData.begin() + indexes.Summary.first, process.mFullData.begin() + indexes.Summary.second),
        //        QStringView(process.mFullData.begin() + indexes.Body.first, process.mFullData.begin() + indexes.Body.second)
        //    );
        //}

        return std::pair<std::vector<CommitData>, QString>{std::move(commitData), std::move(fullData)};
    }

    void Git::PopulateData(CommitModel& model/*, cppgit2::commit& aCommit*/)
    {
        auto commitData = GetCommitData(mRepoPath, mPathSpec);

        model.populateData(std::move(commitData.first), std::move(commitData.second));
    }

    Git::~Git()
    {

    }
}