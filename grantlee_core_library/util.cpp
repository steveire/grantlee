
#include "util_p.h"

Util::Util()
{

}

QString Util::unescapeStringLiteral(const QString &input)
{
  return input.mid(1, input.size() - 2 )
              .replace("\\\'", "'")
              .replace("\\\"", "\"")
              .replace("\\\\", "\\");
}

bool Util::variantIsTrue(const QVariant &variant)
{

  if (!variant.isValid())
    return false;

  switch (variant.type())
  {
  case QVariant::Bool:
  {
    return variant.toBool();
    break;
  }
  case QVariant::String:
  {
    return !variant.toString().isEmpty();
    break;
  }
  case QVariant::Int:
  {
    return (variant.toInt() > 0);
  }
  default:
    break;
  }

//     if it contains QObject pointer
//     {
//       if pointer is null:
//         return false;
//       return true;
//     }

//     if (it contains number)
//     {
//       //       QString varString = var.toString();
//       if (number == 0)
//         return false;
//       return true;
//     }
//     if (it contains boolean)
//     {
//       return boolean;
//     }
//     if (it contains collection)
//     {
//       return collection.size() > 0;
//     }
//
//     if (it contains string)
//     {
//       QString varString = var.toString();
//       if (varString.isEmpty())
//         return false;
//       return true;
//     }
// etc for QPoint etc.

  return false;


}


