/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_COPY_FOLDER_H
#define TEST_COPY_FOLDER_H

#include "autotest.h"

class TestCopyFolder : public QObject
{
    Q_OBJECT
private slots:
    void testCreateFile();
    void testCopyFile();
    void testBackupFolder1();
    void testBackupFolder3();
    void testCopyFolderHard();
};

DECLARE_TEST(TestCopyFolder)

#endif
