#pragma once

#include "aspose/cells_foss/LoadIssue.h"

#include <vector>

namespace Aspose {
namespace Cells_FOSS {

/// Represents load diagnostics.
class LoadDiagnostics final {
public:
    /// Initializes a new instance of the LoadDiagnostics class.
    LoadDiagnostics();

    /// Gets the issues.
    const std::vector<LoadIssue>& GetIssues() const noexcept;

    /// Gets a value indicating whether repairs.
    bool GetHasRepairs() const noexcept;

    /// Gets a value indicating whether data loss risk.
    bool GetHasDataLossRisk() const noexcept;

    /// Adds an issue.
    /// @param issue The issue to add.
    void Add(const LoadIssue& issue);

private:
    std::vector<LoadIssue> _issues;
};

}}  // namespace Aspose::Cells_FOSS
