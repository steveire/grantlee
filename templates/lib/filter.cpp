
#include "filter.h"

using namespace Grantlee;

Filter::~Filter() {}

void Filter::setStream(Grantlee::OutputStream *stream) { m_stream = stream; }

SafeString Filter::escape(const QString &input) const
{
  return m_stream->escape(input);
}

SafeString Filter::escape(const SafeString &input) const
{
  if (input.isSafe())
    return SafeString(m_stream->escape(input), SafeString::IsSafe);
  return m_stream->escape(input);
}

SafeString Filter::conditionalEscape(const SafeString &input) const
{
  if (!input.isSafe())
    return m_stream->escape(input);
  return input;
}

bool Filter::isSafe() const { return false; }
