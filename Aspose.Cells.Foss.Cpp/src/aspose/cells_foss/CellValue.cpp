#include "aspose/cells_foss/CellValue.h"

#include <stdexcept>

namespace Aspose {
namespace Cells_FOSS {

double CellValue::AsDouble() const
{
    if (auto* p = std::get_if<double>(&_v)) {
        return *p;
    }
    if (auto* p = std::get_if<int>(&_v)) {
        return static_cast<double>(*p);
    }
    throw std::bad_variant_access();
}

int CellValue::AsInteger() const
{
    if (auto* p = std::get_if<int>(&_v)) {
        return *p;
    }
    throw std::bad_variant_access();
}

bool CellValue::AsBool() const
{
    if (auto* p = std::get_if<bool>(&_v)) {
        return *p;
    }
    throw std::bad_variant_access();
}

std::string CellValue::AsString() const
{
    if (auto* p = std::get_if<std::string>(&_v)) {
        return *p;
    }
    throw std::bad_variant_access();
}

DateTime CellValue::AsDateTime() const
{
    if (auto* p = std::get_if<DateTime>(&_v)) {
        return *p;
    }
    throw std::bad_variant_access();
}

}}  // namespace Aspose::Cells_FOSS
