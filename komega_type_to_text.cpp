#include "komega_type_to_text.h"

QString Komega_type_to_text(int type)
{
    if (type == 1)
        return "Base";
    if (type == 5)
        return "AIO";
    if (type == 6)
        return "DIO-PDO";
    if (type == 7)
        return "DIO";
    if (type == 8)
        return "PDO";

    return "undefined";
}
