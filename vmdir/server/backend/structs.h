/*
 * Copyright © 2012-2015 VMware, Inc.  All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the “License”); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS, without
 * warranties or conditions of any kind, EITHER EXPRESS OR IMPLIED.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */



/*
 * Module Name: Directory backend
 *
 * Filename: structs.h
 *
 * Abstract:
 *
 * Private Structures among all backends
 *
 */

typedef struct _VDIR_BACKEND_USN_LIST
{
    PVMDIR_MUTEX        pMutex;
    size_t              iSize;
    USN*                pUSNAry;
    USN                 maxOutstandingUSN;  // max Outstanding USN
    USN                 maxUsedMainUSN;     // max USN used (BECtx.wTxnUSN in pUSNAry list)

} VDIR_BACKEND_USN_LIST;
