#pragma once

#include "aspose/cells_foss/XmlElement.h"

#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace Aspose::Cells_FOSS::Tests::Package {

bool EntryExists(const std::filesystem::path& packagePath, std::string_view entryPath);
std::string ReadEntryText(const std::filesystem::path& packagePath, std::string_view entryPath);
void RewriteEntryText(const std::filesystem::path& packagePath,
                      std::string_view entryPath,
                      const std::function<std::string(std::string)>& rewrite);
void RewriteXmlEntry(const std::filesystem::path& packagePath,
                     std::string_view entryPath,
                     const std::function<void(XmlElement&)>& mutateRoot);
void DeleteEntry(const std::filesystem::path& packagePath, std::string_view entryPath);
void MoveEntry(const std::filesystem::path& packagePath,
               std::string_view sourceEntryPath,
               std::string_view destinationEntryPath);
void CreatePackage(const std::filesystem::path& packagePath,
                   const std::map<std::string, std::string>& entries);
std::vector<std::string> ListEntryNames(const std::vector<std::uint8_t>& zipBytes);

}  // namespace Aspose::Cells_FOSS::Tests::Package
