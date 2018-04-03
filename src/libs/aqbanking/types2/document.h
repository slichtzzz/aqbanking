/**********************************************************
 * This file has been automatically created by "typemaker2"
 * from the file "document.xml".
 * Please do not edit this file, all changes will be lost.
 * Better edit the mentioned source file instead.
 **********************************************************/

#ifndef DOCUMENT_H
#define DOCUMENT_H


#ifdef __cplusplus
extern "C" {
#endif

/** @page P_AB_DOCUMENT Structure AB_DOCUMENT
<p>This page describes the properties of AB_DOCUMENT.</p>



<h1>AB_DOCUMENT</h1>



@anchor AB_DOCUMENT_id
<h2>id</h2>

<p>Set this property with @ref AB_Document_SetId(), get it with @ref AB_Document_GetId().</p>


@anchor AB_DOCUMENT_ownerId
<h2>ownerId</h2>

The meaning of this field depends on the type of document. For electronic statements this is the unique id of the account this statement belongs to.
<p>Set this property with @ref AB_Document_SetOwnerId(), get it with @ref AB_Document_GetOwnerId().</p>


@anchor AB_DOCUMENT_mimeType
<h2>mimeType</h2>

<p>Set this property with @ref AB_Document_SetMimeType(), get it with @ref AB_Document_GetMimeType().</p>


@anchor AB_DOCUMENT_data
<h2>data</h2>

<p>Set this property with @ref AB_Document_SetData(), get it with @ref AB_Document_GetData().</p>

*/

/* needed system headers */
#include <gwenhywfar/types.h>
#include <gwenhywfar/list1.h>
#include <gwenhywfar/list2.h>
#include <gwenhywfar/db.h>
#include <gwenhywfar/xml.h>

/* pre-headers */
#include <aqbanking/error.h>
#include <gwenhywfar/types.h>
#include <gwenhywfar/bindata.h>
#include <gwenhywfar/base64.h>

typedef struct AB_DOCUMENT AB_DOCUMENT;
GWEN_LIST_FUNCTION_LIB_DEFS(AB_DOCUMENT, AB_Document, AQBANKING_API)
GWEN_LIST2_FUNCTION_LIB_DEFS(AB_DOCUMENT, AB_Document, AQBANKING_API)



/* post-headers */


/** Constructor. */
AQBANKING_API AB_DOCUMENT *AB_Document_new(void);

/** Destructor. */
AQBANKING_API void AB_Document_free(AB_DOCUMENT *p_struct);

AQBANKING_API void AB_Document_Attach(AB_DOCUMENT *p_struct);

AQBANKING_API AB_DOCUMENT *AB_Document_dup(const AB_DOCUMENT *p_struct);

AQBANKING_API AB_DOCUMENT *AB_Document_copy(AB_DOCUMENT *p_struct, const AB_DOCUMENT *p_src);

/** Getter.
 * Use this function to get the member "id" (see @ref AB_DOCUMENT_id)
*/
AQBANKING_API const char *AB_Document_GetId(const AB_DOCUMENT *p_struct);

/** Getter.
 * Use this function to get the member "ownerId" (see @ref AB_DOCUMENT_ownerId)
*/
AQBANKING_API uint32_t AB_Document_GetOwnerId(const AB_DOCUMENT *p_struct);

/** Getter.
 * Use this function to get the member "mimeType" (see @ref AB_DOCUMENT_mimeType)
*/
AQBANKING_API const char *AB_Document_GetMimeType(const AB_DOCUMENT *p_struct);

/** Getter.
 * Use this function to get the member "data" (see @ref AB_DOCUMENT_data)
*/
AQBANKING_API GWEN_BINDATA AB_Document_GetData(const AB_DOCUMENT *p_struct);

/** Setter.
 * Use this function to set the member "id" (see @ref AB_DOCUMENT_id)
*/
AQBANKING_API void AB_Document_SetId(AB_DOCUMENT *p_struct, const char *p_src);

/** Setter.
 * Use this function to set the member "ownerId" (see @ref AB_DOCUMENT_ownerId)
*/
AQBANKING_API void AB_Document_SetOwnerId(AB_DOCUMENT *p_struct, uint32_t p_src);

/** Setter.
 * Use this function to set the member "mimeType" (see @ref AB_DOCUMENT_mimeType)
*/
AQBANKING_API void AB_Document_SetMimeType(AB_DOCUMENT *p_struct, const char *p_src);

/** Setter.
 * Use this function to set the member "data" (see @ref AB_DOCUMENT_data)
*/
AQBANKING_API void AB_Document_SetData(AB_DOCUMENT *p_struct, GWEN_BINDATA p_src);

/* list1 functions */
AQBANKING_API AB_DOCUMENT_LIST *AB_Document_List_dup(const AB_DOCUMENT_LIST *p_src);

AQBANKING_API void AB_Document_ReadDb(AB_DOCUMENT *p_struct, GWEN_DB_NODE *p_db);

AQBANKING_API int AB_Document_WriteDb(const AB_DOCUMENT *p_struct, GWEN_DB_NODE *p_db);

AQBANKING_API AB_DOCUMENT *AB_Document_fromDb(GWEN_DB_NODE *p_db);

AQBANKING_API int AB_Document_toDb(const AB_DOCUMENT *p_struct, GWEN_DB_NODE *p_db);

AQBANKING_API void AB_Document_ReadXml(AB_DOCUMENT *p_struct, GWEN_XMLNODE *p_db);

AQBANKING_API void AB_Document_WriteXml(const AB_DOCUMENT *p_struct, GWEN_XMLNODE *p_db);

AQBANKING_API void AB_Document_toXml(const AB_DOCUMENT *p_struct, GWEN_XMLNODE *p_db);

AQBANKING_API AB_DOCUMENT *AB_Document_fromXml(GWEN_XMLNODE *p_db);

AQBANKING_API int AB_Document_List2_freeAll(AB_DOCUMENT_LIST2 *tl);
/* end-headers */


#ifdef __cplusplus
}
#endif

#endif
