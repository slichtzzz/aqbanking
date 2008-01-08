/***************************************************************************
 $RCSfile$
 -------------------
 begin       : Mon Jan 07 2008
 copyright   : (C) 2008 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/


#ifndef AIO_OFX_G_IGNORE_P_H
#define AIO_OFX_G_IGNORE_P_H


#include "g_ignore_l.h"

#include <gwenhywfar/stringlist.h>



typedef struct AIO_OFX_GROUP_IGNORE AIO_OFX_GROUP_IGNORE;
struct AIO_OFX_GROUP_IGNORE {
  GWEN_STRINGLIST *openTags;
};


void GWENHYWFAR_CB AIO_OfxGroup_Ignore_FreeData(void *bp, void *p);


int AIO_OfxGroup_Ignore_StartTag(AIO_OFX_GROUP *g, const char *tagName);
int AIO_OfxGroup_Ignore_EndTag(AIO_OFX_GROUP *g, const char *tagName);
int AIO_OfxGroup_Ignore_AddData(AIO_OFX_GROUP *g, const char *data);

#endif

