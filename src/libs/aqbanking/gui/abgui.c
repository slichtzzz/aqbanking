/***************************************************************************
 begin       : Thu Jun 18 2009
 copyright   : (C) 2009 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 * This file is part of the project "AqBanking".                           *
 * Please see toplevel file COPYING of that project for license details.   *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "abgui_p.h"

#include <gwenhywfar/mdigest.h>
#include <gwenhywfar/text.h>
#include <gwenhywfar/misc.h>
#include <gwenhywfar/debug.h>


GWEN_INHERIT(GWEN_GUI, AB_GUI)




GWEN_GUI *AB_Gui_new(AB_BANKING *ab) {
  GWEN_GUI *gui;
  AB_GUI *xgui;

  gui=GWEN_Gui_new();
  GWEN_NEW_OBJECT(AB_GUI, xgui);
  GWEN_INHERIT_SETDATA(GWEN_GUI, AB_GUI, gui, xgui, AB_Gui_FreeData);

  xgui->banking=ab;
  xgui->checkCertFn=GWEN_Gui_SetCheckCertFn(gui, AB_Gui_CheckCert);

  return gui;
}



void AB_Gui_Extend(GWEN_GUI *gui, AB_BANKING *ab) {
  AB_GUI *xgui;

  assert(gui);
  GWEN_NEW_OBJECT(AB_GUI, xgui);
  GWEN_INHERIT_SETDATA(GWEN_GUI, AB_GUI, gui, xgui, AB_Gui_FreeData);

  xgui->banking=ab;
  xgui->checkCertFn=GWEN_Gui_SetCheckCertFn(gui, AB_Gui_CheckCert);
}



void AB_Gui_FreeData(void *bp, void *p) {
  AB_GUI *xgui;

  xgui=(AB_GUI*) p;
  assert(xgui);
  GWEN_FREE_OBJECT(xgui);
}



int AB_Gui__HashPair(const char *token,
		     const char *pin,
		     GWEN_BUFFER *buf) {
  GWEN_MDIGEST *md;
  int rv;

  /* hash token and pin */
  md=GWEN_MDigest_Md5_new();
  rv=GWEN_MDigest_Begin(md);
  if (rv==0)
    rv=GWEN_MDigest_Update(md, (const uint8_t*)token, strlen(token));
  if (rv==0)
    rv=GWEN_MDigest_Update(md, (const uint8_t*)pin, strlen(pin));
  if (rv==0)
    rv=GWEN_MDigest_End(md);
  if (rv<0) {
    DBG_ERROR(AQBANKING_LOGDOMAIN, "Hash error (%d)", rv);
    GWEN_MDigest_free(md);
    return rv;
  }

  GWEN_Text_ToHexBuffer((const char*)GWEN_MDigest_GetDigestPtr(md),
			GWEN_MDigest_GetDigestSize(md),
			buf,
			0, 0, 0);
  GWEN_MDigest_free(md);
  return 0;
}




int AB_Gui_CheckCert(GWEN_GUI *gui,
		     const GWEN_SSLCERTDESCR *cd,
		     GWEN_IO_LAYER *io, uint32_t guiid) {
  AB_GUI *xgui;
  const char *hash;
  const char *status;
  GWEN_BUFFER *hbuf;
  GWEN_DB_NODE *dbCerts=NULL;
  int rv;
  int result=GWEN_ERROR_USER_ABORTED;

  assert(gui);
  xgui=GWEN_INHERIT_GETDATA(GWEN_GUI, AB_GUI, gui);
  assert(xgui);

  hash=GWEN_SslCertDescr_GetFingerPrint(cd);
  status=GWEN_SslCertDescr_GetStatusText(cd);

  hbuf=GWEN_Buffer_new(0, 64, 0, 1);
  AB_Gui__HashPair(hash, status, hbuf);

  DBG_ERROR(0, "Looking for cert %s", GWEN_Buffer_GetStart(hbuf));

  DBG_ERROR(0, "Locking certs");
  rv=AB_Banking_LockSharedConfig(xgui->banking, "certs", guiid);
  if (rv<0) {
    /* fallback */
    DBG_WARN(AQBANKING_LOGDOMAIN, "Could not lock certs db, asking user (%d)", rv);
    result=xgui->checkCertFn(gui, cd, io, guiid);
  }
  else {
    int i;

    DBG_ERROR(0, "Loading certs");
    rv=AB_Banking_LoadSharedConfig(xgui->banking, "certs", &dbCerts, guiid);
    if (rv<0) {
      DBG_INFO(AQBANKING_LOGDOMAIN, "Could not load certs (%d)", rv);
      dbCerts=GWEN_DB_Group_new("certs");
    }

    /* lookup cert or ask */
    DBG_ERROR(0, "Looking up cert");
    i=GWEN_DB_GetIntValue(dbCerts, GWEN_Buffer_GetStart(hbuf), 0, 1);
    if (i==0) {
      DBG_NOTICE(AQBANKING_LOGDOMAIN,
		 "Automatically accepting certificate [%s]",
		 hash);
      result=0;
    }
    else {
      DBG_ERROR(0, "Asking user (intvalue was: %d)", i);
      if (xgui->checkCertFn) {
	result=xgui->checkCertFn(gui, cd, io, guiid);
	if (result==0) {
	  GWEN_DB_SetIntValue(dbCerts, GWEN_DB_FLAGS_OVERWRITE_VARS,
			      GWEN_Buffer_GetStart(hbuf), result);
	}
      }
    }

    /* write new certs */
    DBG_ERROR(0, "Saving certs");
    rv=AB_Banking_SaveSharedConfig(xgui->banking, "certs", dbCerts, guiid);
    if (rv<0) {
      DBG_WARN(AQBANKING_LOGDOMAIN, "Could not unlock certs db (%d)", rv);
    }

    /* unlock certs */
    DBG_ERROR(0, "Unlocking certs");
    rv=AB_Banking_UnlockSharedConfig(xgui->banking, "certs", guiid);
    if (rv<0) {
      DBG_NOTICE(AQBANKING_LOGDOMAIN, "Could not unlock certs db (%d)", rv);
    }
  }

  GWEN_Buffer_free(hbuf);

  DBG_ERROR(0, "Returning %d", result);

  return result;
}

