/*
 *  file:    hjsTest.c
 *
 *  purpose: Testprogram to test db access (DbLib)and use of hash 
 *           functions (glib) 
 *
 *
 *
 *  Ver  Date     Author              Changes
 * ----------------------------------------------------------------------
 *  1.0  18.08.99 Hansjoerg Schmid    original
 * ----------------------------------------------------------------------
 */

/*INCLUDES -------------------------------------------------------------*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* Gnu Library */
#include <glib.h>

/* Project includes */
#include <Debug.h>
#include <LogLib.h>
#include <DbLib.h>  
#include <hjsTest.h>


namesMapList  namesMapListStore;
GHashTable    *namesMapHashStore;  /* hash table handle */


namesMapList *CreateNamesMapList(int numberOfRows)
{
    namesMapList *namesmaplist;
 
    namesmaplist = malloc(sizeof(namesMapList));

    namesmaplist->namesMapList_len = numberOfRows;
    namesmaplist->namesMapList_val = malloc(sizeof(namesMap) * namesmaplist->namesMapList_len);
    

    return(namesmaplist);
}


void NamesMap2HashKey(hashKeyMap *hashkeymap, char *hashString) 
{
/* construct a string where we can run the String hash function */
   sprintf(hashString, "%s/%s", 
	   hashkeymap->terminalId,
	   hashkeymap->moduleIpAddress);

   LogDebugMessage ("NamesMap2HashKey HashKey:%s", hashString);
}


guint NamesMapHashFunc(gconstpointer key) {
   hashKeyMap *hashkeymap;
   char	        hashString[_TCGNAMESIZE*2];

   hashkeymap = (hashKeyMap *)key;
   
   /* construct a string where we can run the String hash function */
   sprintf(hashString, "%s/%s", 
	   hashkeymap->terminalId,
	   hashkeymap->moduleIpAddress);

   return(g_str_hash( (gconstpointer)hashString ));
}

namesMap *NamesGetHash (hashKeyMap *hashkeymap) {
   char	         *hashString;
   int		 hashKeyLen=36;

   hashString = (char *)calloc(1, hashKeyLen);
   NamesMap2HashKey(hashkeymap, hashString);
   return g_hash_table_lookup(namesMapHashStore, hashString);
}

/* ---------------------------------------------------------------------*
 * Function Name: StoreNamesMapList(namesMapList)                       *
 *                                                                      *
 * Description:  Stores NamesMapList and puts map in a hash             *
 *                                                                      *
 * Arguments: namesmaplist: NamesMap read from database                 *
 *                                                                      *
 * Returns: nothing                                                     *
 *                                                                      *
 * Side Effects:                                                        *
 *                                                                      *
 -----------------------------------------------------------------------*/
void StoreNamesMapList(namesMapList *namesmaplist,hashKeyMap *hashkeymap, namesMap *namesmap)
{
   char	         *hashString;
   int		 hashKeyLen=36;

   LogMessage("StoreNamesMapList()");
   

   /* Freeze hash table to insert all elements 
      This optimizes speed */
   g_hash_table_freeze(namesMapHashStore);

   /* Insert the passed element into hash list */
   hashString = (char *)calloc(1, hashKeyLen);
   NamesMap2HashKey(hashkeymap, hashString);
   LogMessage("the key is: %s",hashString);
   g_hash_table_insert(namesMapHashStore, hashString, namesmap);

   /* Make hash resizable again */
   g_hash_table_thaw(namesMapHashStore);

   LogMessage("HashTableSize: %d", g_hash_table_size(namesMapHashStore));
}

/* ---------------------------------------------------------------------*
 * Function Name:  Main                                                 *
 *                                                                      *
 * Description:                                                         *
 *                                                                      *
 * Arguments:                                                           *
 *                                                                      *
 * Returns:                                                             *
 *                                                                      *
 * Side Effects:                                                        *
 *                                                                      *
 -----------------------------------------------------------------------*/

int
main(int argc, char ** argv, char ** envp)

     /* structure used for DB access:

typedef struct {
  DbConn dbConn;
  int errorNbr;
  int columns;
  int rows;
  char *data[MAX_COLUMNS][MAX_ROWS];
} DbAnswerDesc;

*/

{

  DbAnswer dbAnswer;
  DbConn  dbConn1;
  int rows;
  int columns;
  char cmd[_BUFSIZE];
  namesMapList *namesmaplist;
  namesMap    *namesmap, *retPtr;
  hashKeyMap hashKeyMapStruct;
  hashKeyMap *phash = &hashKeyMapStruct;

  LogOpen("hjsTest");
  LogMessage("Test Program Start");    
  LogDebug(10);
  DbInit("");

  dbAnswer = DbConnect("A8620DB_S", "s8610_71_scen", "MickeyMouse", "DbLib");
  if (dbAnswer->errorNbr != 0) {
    LogMessage("DbConnect returns : %s", DbGetErrString(dbAnswer->errorNbr));   
 
    return -1;
  }

  dbConn1 = dbAnswer->dbConn;

  LogMessage("db connection is server:%s|user:%s|password:%s|appl:%s",
        DbGetVar(dbConn1, DBLIB_SERVER_NAME),
        DbGetVar(dbConn1, DBLIB_USER_NAME),
        DbGetVar(dbConn1, DBLIB_PASSWORD_NAME),
        DbGetVar(dbConn1, DBLIB_APPL_NAME));
 
 sprintf(cmd,NAMES_SELECT_COMMAND,"V51_RegrRolf21");
 LogMessage(cmd);    
 
 DbSelect(dbConn1, cmd);
  if (dbAnswer->errorNbr == 0) {
    LogMessage("DbSelect returns columns:%d|rows:%d|time:%d", dbAnswer->columns,
    dbAnswer->rows, time(NULL));

    namesmaplist = CreateNamesMapList(dbAnswer->rows);
    namesmap = namesmaplist->namesMapList_val;
    namesMapListStore = *namesmaplist;
    namesMapHashStore = g_hash_table_new(g_str_hash, g_str_equal);

    /*columns: terminalName,cgName,terminalID,moduleIpAddress */
    for (rows=0; rows<dbAnswer->rows; rows++) {
      for (columns=0; columns<dbAnswer->columns; columns++) {

        LogMessage("DbSelect data col:%d|row:%d|data:%s",
                columns, rows, dbAnswer->data[columns][rows]);
      }
      sprintf(namesmap->terminalName,dbAnswer->data[0][rows]);
      sprintf(namesmap->cgName,dbAnswer->data[1][rows]);
      sprintf(hashKeyMapStruct.terminalId,dbAnswer->data[2][rows]);
      sprintf(hashKeyMapStruct.moduleIpAddress,dbAnswer->data[3][rows]);
      StoreNamesMapList(namesmaplist,phash,namesmap);
      namesmap++;
    }

      dbAnswer = DbFreeSelectData(dbConn1, dbAnswer);
  }          
  else {
    LogMessage("DbSelect returns : %d", dbAnswer->errorNbr);
  }

  /* test if we get the right data back */
  sprintf(phash->terminalId,"1");
  sprintf(phash->moduleIpAddress,"130.198.18.226");
  retPtr =   NamesGetHash (phash);
  LogMessage("CGname: %s   terminalName:  %s",retPtr->cgName,retPtr->terminalName);



  dbAnswer = DbDisconnect(dbConn1, "");

  LogMessage("Test Program Done");    
  return(0);
  
} /* End of main */

/* End of DbLibTest.c */

