/***************************************************************************
    begin       : Tue Dec 31 2013
    copyright   : (C) 2018 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
# include <config.h>
#endif


#include "jobsepaxfersingle_p.h"
#include "jobtransferbase_l.h"
#include "aqhbci_l.h"
#include "accountjob_l.h"
#include "aqhbci/joblayer/job_l.h"
#include "aqhbci/banking/user_l.h"
#include "aqhbci/banking/provider_l.h"
#include "aqhbci/applayer/hhd_l.h"

#include <gwenhywfar/debug.h>
#include <gwenhywfar/misc.h>
#include <gwenhywfar/inherit.h>
#include <gwenhywfar/text.h>

#include <assert.h>






/* --------------------------------------------------------------- FUNCTION */
AH_JOB *AH_Job_SepaTransferSingle_new(AB_PROVIDER *pro, AB_USER *u, AB_ACCOUNT *account)
{
  AH_JOB *j;

  j=AH_Job_TransferBase_new("JobSepaTransferSingle",
                            AB_Transaction_TypeTransfer,
                            AB_Transaction_SubTypeStandard,
                            pro, u, account);
  if (!j)
    return 0;

  AH_Job_SetChallengeClass(j, 9);
  AH_Job_SetSupportedCommand(j, AB_Transaction_CommandSepaTransfer);

  /* overwrite some virtual functions */
  AH_Job_SetPrepareFn(j, AH_Job_SepaTransferSingle_Prepare);
  AH_Job_SetAddChallengeParamsFn(j, AH_Job_SepaTransferSingle_AddChallengeParams);
  AH_Job_SetHandleCommandFn(j, AH_Job_TransferBase_HandleCommand_SepaUndated);
  AH_Job_SetGetLimitsFn(j, AH_Job_TransferBase_GetLimits_SepaUndated);

  return j;
}



/* --------------------------------------------------------------- FUNCTION */
int AH_Job_SepaTransferSingle_AddChallengeParams(AH_JOB *j, int hkTanVer, GWEN_DB_NODE *dbMethod)
{
  const AB_TRANSACTION *t;
  const char *s;
  int tanVer=AH_JOB_TANVER_1_4;

  DBG_ERROR(AQHBCI_LOGDOMAIN, "AddChallengeParams function called");

  t=AH_Job_GetFirstTransfer(j);
  if (t==NULL) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "No validated transaction");
    return GWEN_ERROR_INVALID;
  }

  s=GWEN_DB_GetCharValue(dbMethod, "zkaTanVersion", 0, NULL);
  if (s && *s && strncasecmp(s, "1.3", 3)==0) {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "TAN version is 1.3 (%s)", s);
    tanVer=AH_JOB_TANVER_1_3;
  }

  if (tanVer==AH_JOB_TANVER_1_4) {
    int rv;

    DBG_ERROR(AQHBCI_LOGDOMAIN, "TAN version is 1.4.x");
    rv=AH_HHD14_AddChallengeParams_09(j,
                                      AB_Transaction_GetValue(t),
                                      AB_Transaction_GetRemoteIban(t));
    if (rv<0) {
      DBG_INFO(AQHBCI_LOGDOMAIN, "here (%d)", rv);
      return rv;
    }
  }
  else {
    DBG_ERROR(AQHBCI_LOGDOMAIN, "Unhandled tan version %d for now", tanVer);
    return GWEN_ERROR_INTERNAL;
  }
  return 0;
}



/* --------------------------------------------------------------- FUNCTION */
int AH_Job_SepaTransferSingle_Prepare(AH_JOB *j)
{
  int rv;

  DBG_INFO(AQHBCI_LOGDOMAIN, "Preparing transfer");

  /* select pain profile from group "001" */
  rv=AH_Job_TransferBase_SelectPainProfile(j, 1);
  if (rv<0) {
    DBG_INFO(AQHBCI_LOGDOMAIN, "here (%d)", rv);
    return rv;
  }

  /* export transfers to SEPA */
  rv=AH_Job_TransferBase_SepaExportTransactions(j);
  if (rv<0) {
    DBG_INFO(AQHBCI_LOGDOMAIN, "here (%d)", rv);
    return rv;
  }

  return 0;
}







