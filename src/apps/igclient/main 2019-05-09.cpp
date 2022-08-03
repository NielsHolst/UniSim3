#define IGLIB_DLL dllimport

#include <iostream>
#include <QString>
#include <base/exception.h>
#include <iglib/iglib.h>

using namespace ig;
using namespace std;

int main(int, char **)
{
    int result = 0;
    try {
        Query q;
        q.timeStamp.dayOfYear = 130;
        q.timeStamp.timeOfDay = 12;
        q.timeStamp.timeZone = 1;

        Response r = compute(q);

        if (r.hasError) {
            cout << "\n\nError in iglib:\n" << r.error << "\n";
        }
        else
            cout << responseToString(r);
    }
    catch (const base::Exception &ex) {
        cout << qPrintable(ex.what());
        result = 1;
    }
    catch (const std::exception &ex) {
        cout << ex.what();
        result = 2;
    }
    cout << "\n\nDone!\nHit <Enter>";
    char s[8];
    cin.getline(s,7);
    return result;
}
