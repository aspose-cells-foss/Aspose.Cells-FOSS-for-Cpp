#pragma once
#include "DateTime.h"
#include <cstdint>
#include <string>
#include <string_view>
#include <variant>

namespace Aspose {
namespace Cells_FOSS {

class CellValue {
public:
    using Variant = std::variant<std::monostate, int, double, bool, std::string, DateTime>;

    CellValue() = default;
    CellValue(int v) : _v(v) {}
    CellValue(double v) : _v(v) {}
    CellValue(bool v) : _v(v) {}
    CellValue(const char* v) : _v(std::string(v == nullptr ? "" : v)) {}
    CellValue(const std::string& v) : _v(v) {}
    CellValue(std::string_view v) : _v(std::string(v)) {}
    CellValue(const DateTime& v) : _v(v) {}

    bool IsEmpty()   const { return std::holds_alternative<std::monostate>(_v); }
    bool IsInteger() const { return std::holds_alternative<int>(_v); }
    bool IsDouble()  const { return std::holds_alternative<double>(_v); }
    bool IsBool()    const { return std::holds_alternative<bool>(_v); }
    bool IsString()  const { return std::holds_alternative<std::string>(_v); }
    bool IsDateTime()const { return std::holds_alternative<DateTime>(_v); }

    double AsDouble() const;
    int    AsInteger() const;
    bool   AsBool()    const;
    std::string AsString() const;
    DateTime AsDateTime() const;

private:
    Variant _v;
};

}}  // namespace Aspose::Cells_FOSS