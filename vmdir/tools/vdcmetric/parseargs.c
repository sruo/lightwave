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



#include "includes.h"


#ifndef _WIN32
static const struct option g_long_options[] =
{
        {VDCMETRIC_LONG_OPTION_NETWORK_ADDRESS, required_argument,  0,  VDCMETRIC_OPTION_NETWORK_ADDRESS},
        {VDCMETRIC_LONG_OPTION_DOMAIN,          required_argument,  0,  VDCMETRIC_OPTION_DOMAIN},
        {VDCMETRIC_LONG_OPTION_USERNAME,        required_argument,  0,  VDCMETRIC_OPTION_USERNAME},
        {VDCMETRIC_LONG_OPTION_PASSWORD,        required_argument,  0,  VDCMETRIC_OPTION_PASSWORD},
        {VDCMETRIC_LONG_OPTION_ENABLE,          no_argument,        0,  VDCMETRIC_OPTION_ENABLE},
        {VDCMETRIC_LONG_OPTION_IS_ENABLED,      no_argument,        0,  VDCMETRIC_OPTION_IS_ENABLED},
        {VDCMETRIC_LONG_OPTION_DISABLE,         no_argument,        0,  VDCMETRIC_OPTION_DISABLE},
        {VDCMETRIC_LONG_OPTION_SET_SIZE,        required_argument,  0,  VDCMETRIC_OPTION_SET_SIZE},
        {VDCMETRIC_LONG_OPTION_GET_SIZE,        no_argument,        0,  VDCMETRIC_OPTION_GET_SIZE},
        {VDCMETRIC_LONG_OPTION_RETRIEVE,        no_argument,        0,  VDCMETRIC_OPTION_RETRIEVE},
        {VDCMETRIC_LONG_OPTION_FLUSH,           no_argument,        0,  VDCMETRIC_OPTION_FLUSH},
        {VDCMETRIC_LONG_OPTION_AGGREGATE,       no_argument,        0,  VDCMETRIC_OPTION_AGGREGATE},
        {VDCMETRIC_LONG_COLUMN_LOGIN_DN,        no_argument,        0,  VDCMETRIC_COLUMN_LOGIN_DN},
        {VDCMETRIC_LONG_COLUMN_IP,              no_argument,        0,  VDCMETRIC_COLUMN_IP},
        {VDCMETRIC_LONG_COLUMN_PORT,            no_argument,        0,  VDCMETRIC_COLUMN_PORT},
        {VDCMETRIC_LONG_COLUMN_OPERATION,       no_argument,        0,  VDCMETRIC_COLUMN_OPERATION},
        {VDCMETRIC_LONG_COLUMN_STRING,          no_argument,        0,  VDCMETRIC_COLUMN_STRING},
        {VDCMETRIC_LONG_COLUMN_ERROR_CODE,      no_argument,        0,  VDCMETRIC_COLUMN_ERROR_CODE},
        {VDCMETRIC_LONG_COLUMN_TIME,            no_argument,        0,  VDCMETRIC_COLUMN_TIME},
        {0, 0, 0, 0}
};
#endif


static
DWORD
_checkAndSetOperation(
        PBOOLEAN pOperation,
        PBOOLEAN pIsOperationSet
        )
{
    DWORD dwError = 0;
    if (*pIsOperationSet)
    {
        printf("You can only pass one operation at a time\n");
        dwError = ERROR_INVALID_PARAMETER;
    }
    else
    {
        *pOperation = TRUE;
        *pIsOperationSet = TRUE;
    }
    return dwError;
}

DWORD
VmDirParseBaseArgs(
        int      argc,
        char*    argv[],
        PSTR* ppszNetworkAddress,
        PSTR* ppszDomain,
        PSTR* ppszUserName,
        PSTR* ppszPassword
        )
{
    DWORD dwError = ERROR_SUCCESS;

#ifndef _WIN32
    int opt = 0;
    int opt_idx = 0;
#else
    int i = 1;
#endif

    PSTR pszNetworkAddress = NULL;
    PSTR pszDomain = NULL;
    PSTR pszUserName = NULL;
    PSTR pszPassword = NULL;

    if (ppszNetworkAddress == NULL
            || ppszDomain == NULL
            || ppszUserName == NULL
            || ppszPassword == NULL)
    {
        dwError = ERROR_INVALID_PARAMETER;
        BAIL_ON_VMDIR_ERROR(dwError);
    }

#ifndef _WIN32
    optind = 1;
    while ((opt = getopt_long(argc, argv, VDCMETRIC_OPTIONS, g_long_options, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case VDCMETRIC_OPTION_NETWORK_ADDRESS:
            pszNetworkAddress = optarg;
            break;

        case VDCMETRIC_OPTION_DOMAIN:
            pszDomain = optarg;
            break;

        case VDCMETRIC_OPTION_USERNAME:
            pszUserName = optarg;
            break;

        case VDCMETRIC_OPTION_PASSWORD:
            pszPassword = optarg;
            break;

        default:
            break;
        }
    }
#else
    while (i < argc)
    {
        if (VmDirIsCmdLineOption(argv[i]))
        {
            if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_NETWORK_ADDRESS, argv[i], TRUE) == 0
                    || VmDirStringCompareA(VDCMETRIC_OPTION_NETWORK_ADDRESS, argv[i], TRUE) == 0)
            {
                VmDirGetCmdLineOption(argc, argv, &i, &pszNetworkAddress);
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_DOMAIN, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_OPTION_DOMAIN, argv[i], TRUE) == 0)
            {
                VmDirGetCmdLineOption(argc, argv, &i, &pszDomain);
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_USERNAME, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_OPTION_USERNAME, argv[i], TRUE) == 0)
            {
                VmDirGetCmdLineOption(argc, argv, &i, &pszUserName);
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_PASSWORD, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_OPTION_PASSWORD, argv[i], TRUE) == 0)
            {
                VmDirGetCmdLineOption(argc, argv, &i, &pszPassword);
            }
        }
        i++;
    }
#endif

    *ppszNetworkAddress = pszNetworkAddress;
    *ppszDomain = pszDomain;
    *ppszUserName = pszUserName;
    *ppszPassword = pszPassword;

cleanup:
    return dwError;

error:
    goto cleanup;
}

DWORD
VmDirParseOperationArgs(
        int      argc,
        char*    argv[],
        PBOOLEAN pbNodeData,
        PBOOLEAN pbEnable,
        PBOOLEAN pbIsEnabled,
        PBOOLEAN pbDisable,
        PBOOLEAN pbSetSize,
        PBOOLEAN pbGetSize,
        PBOOLEAN pbRetrieve,
        PBOOLEAN pbFlush,
        PBOOLEAN pbAggregate
        )
{
    DWORD dwError = ERROR_SUCCESS;
#ifndef _WIN32
    int opt = 0;
    int opt_idx = 0;
#else
    int i = 1;
#endif

    BOOLEAN bIsOperationSet = FALSE;

    BOOLEAN bNodeData   = FALSE;
    BOOLEAN bEnable     = FALSE;
    BOOLEAN bIsEnabled  = FALSE;
    BOOLEAN bDisable    = FALSE;
    BOOLEAN bSetSize    = FALSE;
    BOOLEAN bGetSize    = FALSE;
    BOOLEAN bRetrieve   = FALSE;
    BOOLEAN bFlush      = FALSE;
    BOOLEAN bAggregate  = FALSE;

    if (pbNodeData == NULL
            || pbEnable == NULL
            || pbIsEnabled == NULL
            || pbDisable == NULL
            || pbSetSize == NULL
            || pbGetSize == NULL
            || pbRetrieve == NULL
            || pbFlush == NULL
            || pbAggregate == NULL)
    {
        dwError = ERROR_INVALID_PARAMETER;
        BAIL_ON_VMDIR_ERROR(dwError);
    }

#ifndef _WIN32
    optind = 1;
    while ((opt = getopt_long(argc, argv, VDCMETRIC_OPTIONS, g_long_options, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case VDCMETRIC_OPTION_NODE_DATA:
            dwError = _checkAndSetOperation(&bNodeData, &bIsOperationSet);
            BAIL_ON_VMDIR_ERROR(dwError);
            break;

        case VDCMETRIC_OPTION_ENABLE:
            dwError = _checkAndSetOperation(&bEnable, &bIsOperationSet);
            BAIL_ON_VMDIR_ERROR(dwError);
            break;

        case VDCMETRIC_OPTION_IS_ENABLED:
            dwError = _checkAndSetOperation(&bIsEnabled, &bIsOperationSet);
            BAIL_ON_VMDIR_ERROR(dwError);
            break;

        case VDCMETRIC_OPTION_DISABLE:
            dwError = _checkAndSetOperation(&bDisable, &bIsOperationSet);
            BAIL_ON_VMDIR_ERROR(dwError);
            break;

        case VDCMETRIC_OPTION_SET_SIZE:
            dwError = _checkAndSetOperation(&bSetSize, &bIsOperationSet);
            BAIL_ON_VMDIR_ERROR(dwError);
            break;

        case VDCMETRIC_OPTION_GET_SIZE:
            dwError = _checkAndSetOperation(&bGetSize, &bIsOperationSet);
            BAIL_ON_VMDIR_ERROR(dwError);
            break;

        case VDCMETRIC_OPTION_RETRIEVE:
            dwError = _checkAndSetOperation(&bRetrieve, &bIsOperationSet);
            BAIL_ON_VMDIR_ERROR(dwError);
            break;

        case VDCMETRIC_OPTION_FLUSH:
            dwError = _checkAndSetOperation(&bFlush, &bIsOperationSet);
            BAIL_ON_VMDIR_ERROR(dwError);
            break;

        case VDCMETRIC_OPTION_AGGREGATE:
            dwError = _checkAndSetOperation(&bAggregate, &bIsOperationSet);
            BAIL_ON_VMDIR_ERROR(dwError);
            break;

        default:
            break;
        }
    }
#else
    while (i < argc)
    {
        if (VmDirIsCmdLineOption(argv[i]))
        {
            if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_NODE_DATA, argv[i], TRUE) == 0
                    || VmDirStringCompareA(VDCMETRIC_OPTION_NODE_DATA, argv[i], TRUE) == 0)
            {
                dwError = _checkAndSetOperation(&bNodeData, &bIsOperationSet);
                BAIL_ON_VMDIR_ERROR(dwError);
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_ENABLE, argv[i], TRUE) == 0
                    || VmDirStringCompareA(VDCMETRIC_OPTION_ENABLE, argv[i], TRUE) == 0)
            {
                dwError = _checkAndSetOperation(&bEnable, &bIsOperationSet);
                BAIL_ON_VMDIR_ERROR(dwError);
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_IS_ENABLED, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_OPTION_IS_ENABLED, argv[i], TRUE) == 0)
            {
                dwError = _checkAndSetOperation(&bIsEnabled, &bIsOperationSet);
                BAIL_ON_VMDIR_ERROR(dwError);
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_DISABLE, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_OPTION_DISABLE, argv[i], TRUE) == 0)
            {
                dwError = _checkAndSetOperation(&bDisable, &bIsOperationSet);
                BAIL_ON_VMDIR_ERROR(dwError);
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_SET_SIZE, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_OPTION_SET_SIZE, argv[i], TRUE) == 0)
            {
                dwError = _checkAndSetOperation(&bSetSize, &bIsOperationSet);
                BAIL_ON_VMDIR_ERROR(dwError);
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_GET_SIZE, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_OPTION_GET_SIZE, argv[i], TRUE) == 0)
            {
                dwError = _checkAndSetOperation(&bGetSize, &bIsOperationSet);
                BAIL_ON_VMDIR_ERROR(dwError);
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_RETRIEVE, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_OPTION_RETRIEVE, argv[i], TRUE) == 0)
            {
                dwError = _checkAndSetOperation(&bRetrieve, &bIsOperationSet);
                BAIL_ON_VMDIR_ERROR(dwError);
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_FLUSH, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_OPTION_FLUSH, argv[i], TRUE) == 0)
            {
                dwError = _checkAndSetOperation(&bFlush, &bIsOperationSet);
                BAIL_ON_VMDIR_ERROR(dwError);
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_AGGREGATE, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_OPTION_AGGREGATE, argv[i], TRUE) == 0)
            {
                dwError = _checkAndSetOperation(&bAggregate, &bIsOperationSet);
                BAIL_ON_VMDIR_ERROR(dwError);
            }
        }
        i++;
    }
#endif

    *pbNodeData     = bNodeData;
    *pbEnable       = bEnable;
    *pbIsEnabled    = bIsEnabled;
    *pbDisable      = bDisable;
    *pbSetSize      = bSetSize;
    *pbGetSize      = bGetSize;
    *pbRetrieve     = bRetrieve;
    *pbFlush        = bFlush;
    *pbAggregate    = bAggregate;

cleanup:
    return dwError;

error:
    goto cleanup;
}

DWORD
VmDirParseSetSizeArgs(
        int      argc,
        char*    argv[],
        PDWORD pdwSize
        )
{
    DWORD dwError = ERROR_SUCCESS;
#ifndef _WIN32
    int opt = 0;
    int opt_idx = 0;
#else
    int i = 1;
    PSTR pszSize = NULL;
#endif

    DWORD dwSize = 0;

    if (pdwSize == NULL)
    {
        dwError = ERROR_INVALID_PARAMETER;
        BAIL_ON_VMDIR_ERROR(dwError);
    }

#ifndef _WIN32
    optind = 1;
    while ((opt = getopt_long(argc, argv, VDCMETRIC_OPTIONS, g_long_options, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case VDCMETRIC_OPTION_SET_SIZE:
            dwSize = atoi(optarg);
            break;

        default:
            break;
        }
    }
#else
    while (i < argc)
    {
        if (VmDirIsCmdLineOption(argv[i]))
        {
            if (VmDirStringCompareA(VDCMETRIC_LONG_OPTION_SET_SIZE, argv[i], TRUE) == 0
                    || VmDirStringCompareA(VDCMETRIC_OPTION_SET_SIZE, argv[i], TRUE) == 0)
            {
                VmDirGetCmdLineOption(argc, argv, &i, &pszSize);
                dwSize = atoi(pszSize);
            }
        }
        i++;
    }
#endif

    *pdwSize = dwSize;

cleanup:
    return dwError;

error:
    goto cleanup;
}

DWORD
VmDirParseAggregateArgs(
        int      argc,
        char*    argv[],
        PBOOLEAN pbLoginDN,
        PBOOLEAN pbIP,
        PBOOLEAN pbPort,
        PBOOLEAN pbOperation,
        PBOOLEAN pbString,
        PBOOLEAN pbErrorCode,
        PBOOLEAN pbTime
        )
{
    DWORD dwError = ERROR_SUCCESS;
#ifndef _WIN32
    int opt = 0;
    int opt_idx = 0;
#else
    int i = 1;
#endif

    BOOLEAN bLoginDN = FALSE;
    BOOLEAN bIP = FALSE;
    BOOLEAN bPort = FALSE;
    BOOLEAN bOperation = FALSE;
    BOOLEAN bString = FALSE;
    BOOLEAN bErrorCode = FALSE;
    BOOLEAN bTime = FALSE;

    if (pbLoginDN == NULL
            || pbIP == NULL
            || pbPort == NULL
            || pbOperation == NULL
            || pbString == NULL
            || pbErrorCode == NULL
            || pbTime == NULL)
    {
        dwError = ERROR_INVALID_PARAMETER;
        BAIL_ON_VMDIR_ERROR(dwError);
    }

#ifndef _WIN32
    optind = 1;
    while ((opt = getopt_long(argc, argv, VDCMETRIC_OPTIONS, g_long_options, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case VDCMETRIC_COLUMN_LOGIN_DN:
            bLoginDN = TRUE;
            break;

        case VDCMETRIC_COLUMN_IP:
            bIP = TRUE;
            break;

        case VDCMETRIC_COLUMN_PORT:
            bPort = TRUE;
            break;

        case VDCMETRIC_COLUMN_OPERATION:
            bOperation = TRUE;
            break;

        case VDCMETRIC_COLUMN_STRING:
            bString = TRUE;
            break;

        case VDCMETRIC_COLUMN_ERROR_CODE:
            bErrorCode = TRUE;
            break;

        case VDCMETRIC_COLUMN_TIME:
            bTime = TRUE;
            break;

        default:
            break;
        }
    }
#else
    while (i < argc)
    {
        if (VmDirIsCmdLineOption(argv[i]))
        {
            if (VmDirStringCompareA(VDCMETRIC_LONG_COLUMN_LOGIN_DN, argv[i], TRUE) == 0
                    || VmDirStringCompareA(VDCMETRIC_COLUMN_LOGIN_DN, argv[i], TRUE) == 0)
            {
                bLoginDN = TRUE;
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_COLUMN_IP, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_COLUMN_IP, argv[i], TRUE) == 0)
            {
                bIP = TRUE;
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_COLUMN_PORT, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_COLUMN_PORT, argv[i], TRUE) == 0)
            {
                bPort = TRUE;
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_COLUMN_OPERATION, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_COLUMN_OPERATION, argv[i], TRUE) == 0)
            {
                bOperation = TRUE;
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_COLUMN_STRING, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_COLUMN_STRING, argv[i], TRUE) == 0)
            {
                bString = TRUE;
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_COLUMN_ERROR_CODE, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_COLUMN_ERROR_CODE, argv[i], TRUE) == 0)
            {
                bErrorCode = TRUE;
            }
            else if (VmDirStringCompareA(VDCMETRIC_LONG_COLUMN_TIME, argv[i], TRUE) == 0
                        || VmDirStringCompareA(VDCMETRIC_COLUMN_TIME, argv[i], TRUE) == 0)
            {
                bTime = TRUE;
            }
        }
        i++;
    }
#endif

    *pbLoginDN = bLoginDN;
    *pbIP = bIP;
    *pbPort = bPort;
    *pbOperation = bOperation;
    *pbString = bString;
    *pbErrorCode = bErrorCode;
    *pbTime = bTime;

cleanup:
    return dwError;

error:
    goto cleanup;
}

VOID
ShowUsage(
        VOID
        )
{
    printf( "Usage: \n"
            "vdcmetric [-h <network_address> -d <domain> -u <username> -w <password>] ...\n"
            "\n"
            "   [--enable|-E] Enables the node collecting data\n"
            "       vdcmetric --enable\n"
            "                 [-h <network_address>\n"
            "                  -d <domain>\n"
            "                  -u <username>\n"
            "                  -w <password>]\n"
            "\n"
            "   [--isenabled|-I] Checks whether the node is already collecting data\n"
            "       vdcmetric --isenabled\n"
            "                 [-h <network_address>\n"
            "                  -d <domain>\n"
            "                  -u <username>\n"
            "                  -w <password>]\n"
            "\n"
            "   [--disable|-D] Disables the node collecting data\n"
            "       vdcmetric --disable\n"
            "                 [-h <network_address>\n"
            "                  -d <domain>\n"
            "                  -u <username>\n"
            "                  -w <password>]\n"
            "\n"
            "   [--setsize|-S] Configures how much data the node should keep in its buffer\n"
            "       vdcmetric --setsize <capacity>\n"
            "                 [-h <network_address>\n"
            "                  -d <domain>\n"
            "                  -u <username>\n"
            "                  -w <password>]\n"
            "\n"
            "   [--getsize|-G] Gets how much data the node is configured to keep in its buffer\n"
            "       vdcmetric --getsize\n"
            "                 [-h <network_address>\n"
            "                  -d <domain>\n"
            "                  -u <username>\n"
            "                  -w <password>]\n"
            "\n"
            "   [--retrieve|-R] Retrieve collected data from the node and prints in terminal\n"
            "       vdcmetric --retrieve\n"
            "                 [-h <network_address>\n"
            "                  -d <domain>\n"
            "                  -u <username>\n"
            "                  -w <password>]\n"
            "\n"
            "   [--flush|-F] Flushes the node's buffer up to the offset which already has been read\n"
            "       vdcmetric --flush\n"
            "                 [-h <network_address>\n"
            "                  -d <domain>\n"
            "                  -u <username>\n"
            "                  -w <password>]\n"
            "\n"
            "   [--aggregate|-A] Aggregates superlog data and displays by columns: (l) loginDN\n"
            "                                                                      (i) ip\n"
            "                                                                      (p) port\n"
            "                                                                      (o) operation\n"
            "                                                                      (s) string\n"
            "                                                                      (e) errorcode\n"
            "                                                                      (t) time\n"
            "       vdcmetric --aggregate [-l -i -p -o -s -e -t]\n"
            "                 [-h <network_address>\n"
            "                  -d <domain>\n"
            "                  -u <username>\n"
            "                  -w <password>]\n"
    );
}
