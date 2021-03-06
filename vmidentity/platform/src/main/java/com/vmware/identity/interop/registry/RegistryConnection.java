/*
 * Copyright (c) 2012-2015 VMware, Inc.  All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, without
 * warranties or conditions of any kind, EITHER EXPRESS OR IMPLIED.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

/**
 * VMware Identity Service
 * 
 * Registry Connection Object
 * 
 * @author:  Sriram Nambakam <snambakam@vmware.com>
 * 
 * @version: 1.0
 * @since:   2011-12-7
 * 
 */

package com.vmware.identity.interop.registry;

import com.sun.jna.Pointer;

public class RegistryConnection
{
    private Pointer _hConnection;
    
    public RegistryConnection(Pointer hConnection)
    {
        _hConnection = hConnection;
    }
    
    public Pointer getConnection()
    {
        return _hConnection;
    }
    
    public void dispose() 
    { 
        if (_hConnection != null)
        {
            RegistryAdapter.RegClientLibrary.INSTANCE.LwRegCloseServer(_hConnection);
            _hConnection = Pointer.NULL;
        }
    }
    
    protected void finalize() throws Throwable
    { 
        try
        {
            dispose();
        }
        finally
        {
            super.finalize();
        }
    }
}
