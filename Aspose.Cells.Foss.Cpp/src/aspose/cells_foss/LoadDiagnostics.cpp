#include "aspose/cells_foss/LoadDiagnostics.h"

#include <algorithm>

namespace Aspose {
namespace Cells_FOSS {

LoadDiagnostics::LoadDiagnostics() = default;

const std::vector<LoadIssue>& LoadDiagnostics::GetIssues() const noexcept
{
    return _issues;
}

bool LoadDiagnostics::GetHasRepairs() const noexcept
{
    return std::any_of(_issues.begin(), _issues.end(),
                       [](const LoadIssue& issue) { return issue.GetRepairApplied(); });
}

bool LoadDiagnostics::GetHasDataLossRisk() const noexcept
{
    return std::any_of(_issues.begin(), _issues.end(),
                       [](const LoadIssue& issue) { return issue.GetDataLossRisk(); });
}

void LoadDiagnostics::Add(const LoadIssue& issue)
{
    _issues.push_back(issue);
}

}}  // namespace Aspose::Cells_FOSS
