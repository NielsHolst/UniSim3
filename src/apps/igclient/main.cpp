#define IGLIB_DLL dllimport

#include <iostream>
#include <string>
#include <QFileInfo>
#include <QString>
#include <base/box.h>
#include <base/exception.h>
#include <iglib/iglib.h>
#include "query_reader_json.h"

using namespace base;
using namespace ig;
using namespace std;

const QString path =
  "C:/Users/au152367/Documents/QDev/UniSim3/input/projects/ig";
//               "D:/Documents/QDev/UniSim3/input/projects/ig";

const QString fileName =
//"2022-09-06-116";
//"2022-08-30-ver-2-3-63";
//"2022-09-07-ver-3-0-3";
"2022-11-18-par";

void writeBoxScript() {
    QString filePath = path + "/" + fileName + ".box";
    QFile file(filePath);
    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(filePath);
    QTextStream text(&file);
    text << Box::root()->toText("Ia");
}

int main(int, char **)
{
    int result = 0;
    QueryReaderJson reader;
    QString filePath = path + "/" + fileName + ".json";
    try {
        Response r;
        if (QFileInfo::exists(filePath)) {
            cout << "\nParsing...\n";
            Query q = reader.parse(filePath);

            cout << "\nComputing...\n";
            r = compute(q);
            cout << "RESPONSE 1:\n" << responseToString(r) << "\n";
            cout << "\nComputing...\n";
            r = compute(q);
            cout << "\nRESPONSE 2:\n" << responseToString(r) << "\n";
            cout << "Writing boxscript written to " << qPrintable(path) << "\n";
            writeBoxScript();
        }
        else {
            cout << "JSON file not found " << qPrintable(filePath) << std::endl;
        }
    }
    catch (const Exception &ex) {
        cout << qPrintable(ex.what());
        result = 1;
    }
    catch (const std::exception &ex) {
        cout << ex.what();
        result = 2;
    }
    return result;
}
