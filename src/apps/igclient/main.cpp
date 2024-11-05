#define IGLIB_DLL dllimport

#include <iostream>
#include <string>
#include <QCoreApplication>
#include <QFileInfo>
#include <QString>
#include <base/box.h>
#include <base/boxscript_converter.h>
#include <base/dialog_quiet.h>
#include <base/exception.h>
#include <base/mega_factory.h>
#include <iglib/iglib.h>
#include "query_reader_json.h"

using namespace base;
using namespace ig;
using namespace std;

const QString path =
  "C:/MyDocuments/QDev/UniSim3/input/projects/ig";

const QString fileName =
        //"2022-09-06-116";
        //"2022-08-30-ver-2-3-63";
        //"2022-09-07-ver-3-0-3";
        //"2022-11-18-par";
        //"2024-04-25 for meget lys";
        //"2024-06-06 hus 154";
        //"2024-06-13 ingen lamper";
        //"2024-06-27 aranet fejl rettet";
        "2024-10-09 PKM 2";

void writeBoxScript() {
    QString filePath = path + "/" + fileName + ".box";
    QFile file(filePath);
    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(filePath);
    cout << "\nWriting boxscript: "<< qPrintable(filePath) << "\n";
    QTextStream text(&file);
    BoxscriptConverter converter(Box::root(), BoxscriptConverter::Option::WriteUserScript);
    text << converter.toString();
}

int main(int, char **)
{
    new DialogQuiet(qApp);
    int result = 0;
    QueryReaderJson reader;
    QString filePath = path + "/" + fileName + ".json";
    try {
        Response r;
        if (QFileInfo::exists(filePath)) {
            cout << "\nParsing...\n";
            MegaFactory::usingPlugin("vg");
            Query q = reader.parse(filePath);

            cout << "\nComputing...\n";
            r = compute(q);
            cout << "RESPONSE 1:\n" << responseToString(r) << "\n";
            cout << "\nComputing...\n";
            r = compute(q);
            cout << "\nRESPONSE 2:\n" << responseToString(r) << "\n";
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
