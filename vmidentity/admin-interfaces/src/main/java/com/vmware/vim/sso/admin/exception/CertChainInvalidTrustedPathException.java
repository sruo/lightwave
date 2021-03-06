/* **********************************************************************
 * Copyright 2014 VMware, Inc.  All rights reserved.
 * *********************************************************************/

package com.vmware.vim.sso.admin.exception;

/**
 * This exception is thrown by service methods to indicate that the certificate
 * chain is invalid.
 */
public class CertChainInvalidTrustedPathException
    extends ServiceException
{
    private static final long serialVersionUID = -7325876422889330524L;

    private final String name;

    /**
     * Creates a new exception to indicate that the certificate chain
     * is invalid.
     *
     * @param name
     *          Name of the problematic certificate chain.
     */
    public CertChainInvalidTrustedPathException(String name) {
        super(getDefaultMessage(name));
        assert name != null && !name.trim().isEmpty();
        this.name = name;
    }

    public String getName()
    {
        return name;
    }

    private static String getDefaultMessage(String name)
    {
        return String.format("Certpath for [%s] doesn't exist.", name);
    }
}
