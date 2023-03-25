/*
 *  file: hjsTest.h
 *
 *  purpose: headerfile to hjsTest.h
 *
 *
 *  Ver  Date      Author                    Changes
 * ----------------------------------------------------------------------
 *  1.0  18.Aug.99 Hansjoerg Schmid          original
 * ----------------------------------------------------------------------
 */
#define _BUFSIZE 1024
#define _TCGNAMESIZE 50

#define NAMES_SELECT_COMMAND  \
"select terminalName,cgName,terminalID,moduleIpAddress \
from TestTerminal, TestTestModule, TestLink, TestStack, TestCG, Test \
where testName = '%s' \
and TestStack.oid = TestLink.stackID \
and TestStack.cgID = TestCG.oid \
and TestCG.testID = Test.oid \
and TestLink.stackID = TestTerminal.stackID \
and TestLink.tmID = TestTestModule.oid \
order by TestTerminal.terminalID"

typedef struct namesMap  {
    char cgName[_TCGNAMESIZE];
    char terminalName[_TCGNAMESIZE];
} namesMap;

typedef struct hashKeyMap  {
    char terminalId[16];
    char moduleIpAddress[19];
} hashKeyMap;

typedef struct namesMapList {
    int namesMapList_len;
    namesMap *namesMapList_val;
} namesMapList;


/* end of file: hjsTest.h */
