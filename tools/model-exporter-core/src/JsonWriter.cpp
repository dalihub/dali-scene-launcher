#include "JsonWriter.h"

JsonWriter::JsonWriter(std::ostream & stream, const char* indentation)
: mStream(stream),
  mIndentation(indentation ? indentation : "")
{}

void JsonWriter::WriteObject(const char* name, bool oneLiner)
{
  WritePreamble(name);
  if (!mScopes.empty())
  {
    mScopes.back().needsComma = true;
  }

  mScopes.push_back({ Scope::OBJECT, oneLiner, false });
  mStream << '{';
  NewLine(oneLiner);
}

void JsonWriter::WriteArray(const char* name, bool oneLiner)
{
  WritePreamble(name);
  if (!mScopes.empty())
  {
    mScopes.back().needsComma = true;
  }

  mScopes.push_back({ Scope::ARRAY, oneLiner, false });
  mStream << '[';
  NewLine(oneLiner);
}

void JsonWriter::WriteValue(const char* name, const char* value)
{
  WritePreamble(name);
  mScopes.back().needsComma = true;
  if (value)
  {
    mStream << "\"" << value << "\"";
  }
  else
  {
    mStream << "null";
  }
}

void JsonWriter::WriteValue(const char* name, int value)
{
  WritePreamble(name);
  mScopes.back().needsComma = true;
  mStream << value;
}

void JsonWriter::WriteValue(const char* name, unsigned int value)
{
  WritePreamble(name);
  mScopes.back().needsComma = true;
  mStream << value;
}

void JsonWriter::WriteValue(const char* name, double value)
{
  WritePreamble(name);
  mScopes.back().needsComma = true;
  mStream << value;
}

void JsonWriter::WriteValue(const char* name, bool value)
{
  WritePreamble(name);
  mScopes.back().needsComma = true;
  mStream << std::boolalpha << value << std::noboolalpha;
}

void JsonWriter::CloseScope()
{
  const Scope scope = mScopes.back();
  mScopes.pop_back();

  if (scope.needsComma)
  {
    NewLine(scope.isOneLiner);
  }
  Indent(scope.isOneLiner);
  mStream.put(scope.type);
}

void JsonWriter::Comma()
{
  if (!mScopes.empty() && mScopes.back().needsComma)
  {
    mStream.put(',');
    NewLine(mScopes.back().isOneLiner);
  }
}

void JsonWriter::Indent(bool oneLiner)
{
  if (!oneLiner && !mIndentation.empty())
  {
    for (auto& i : mScopes)
    {
      mStream << mIndentation;
    }
  }
}

void JsonWriter::Name(const char* name)
{
  assert((name != nullptr && !mScopes.empty() && mScopes.back().type == Scope::OBJECT) ||
    (name == nullptr && (mScopes.empty() || mScopes.back().type == Scope::ARRAY)));
  if (name)
  {
    mStream << "\"" << name << "\": ";
  }
}

void JsonWriter::NewLine(bool oneLiner)
{
  mStream.put(oneLiner ? ' ' : '\n');
}
