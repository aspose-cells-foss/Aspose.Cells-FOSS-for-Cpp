#include "aspose/cells_foss/core/WorksheetViewModel.h"

namespace Aspose {
namespace Cells_FOSS {
namespace Core {

WorksheetViewModel::WorksheetViewModel()
    : m_showGridLines(true)
    , m_showRowColumnHeaders(true)
    , m_showZeros(true)
    , m_rightToLeft(false)
    , m_zoomScale(100)
{
}

bool WorksheetViewModel::GetShowGridLines() const
{
    return m_showGridLines;
}

void WorksheetViewModel::SetShowGridLines(bool value)
{
    m_showGridLines = value;
}

bool WorksheetViewModel::GetShowRowColumnHeaders() const
{
    return m_showRowColumnHeaders;
}

void WorksheetViewModel::SetShowRowColumnHeaders(bool value)
{
    m_showRowColumnHeaders = value;
}

bool WorksheetViewModel::GetShowZeros() const
{
    return m_showZeros;
}

void WorksheetViewModel::SetShowZeros(bool value)
{
    m_showZeros = value;
}

bool WorksheetViewModel::GetRightToLeft() const
{
    return m_rightToLeft;
}

void WorksheetViewModel::SetRightToLeft(bool value)
{
    m_rightToLeft = value;
}

int WorksheetViewModel::GetZoomScale() const
{
    return m_zoomScale;
}

void WorksheetViewModel::SetZoomScale(int value)
{
    m_zoomScale = value;
}

} // namespace Core
} // namespace Cells_FOSS
} // namespace Aspose
