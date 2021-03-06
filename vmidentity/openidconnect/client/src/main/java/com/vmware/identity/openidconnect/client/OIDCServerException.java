/*
 *  Copyright (c) 2012-2015 VMware, Inc.  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not
 *  use this file except in compliance with the License.  You may obtain a copy
 *  of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, without
 *  warranties or conditions of any kind, EITHER EXPRESS OR IMPLIED.  See the
 *  License for the specific language governing permissions and limitations
 *  under the License.
 */

package com.vmware.identity.openidconnect.client;


/**
 * OIDC server side exception.
 *
 * @author Jun Sun
 */
public class OIDCServerException extends Exception {

    private static final long serialVersionUID = 1L;

    private final OIDCServerError oidcServerError;

    OIDCServerException(String errorCode, String errorDescription) {
        super(String.format(
                "Server error response. Error code: %s; Error description: %s.",
                errorCode,
                errorDescription));

        this.oidcServerError = OIDCServerError.getOIDCServerError(errorCode);
    }

    /**
     * Get OIDC server error
     *
     * @return                          OIDC server error object
     */
    public OIDCServerError getOIDCServerError() {
        return this.oidcServerError;
    }
}
