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

package com.vmware.identity.openidconnect.server;

import java.security.PrivateKey;
import java.security.cert.Certificate;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import org.apache.commons.lang3.Validate;

import com.vmware.identity.idm.Attribute;
import com.vmware.identity.idm.AttributeValuePair;
import com.vmware.identity.idm.DomainType;
import com.vmware.identity.idm.GSSResult;
import com.vmware.identity.idm.IDMLoginException;
import com.vmware.identity.idm.IIdentityStoreData;
import com.vmware.identity.idm.InvalidPrincipalException;
import com.vmware.identity.idm.NoSuchOIDCClientException;
import com.vmware.identity.idm.NoSuchTenantException;
import com.vmware.identity.idm.OIDCClient;
import com.vmware.identity.idm.PersonDetail;
import com.vmware.identity.idm.PersonUser;
import com.vmware.identity.idm.PrincipalId;
import com.vmware.identity.idm.SolutionDetail;
import com.vmware.identity.idm.SolutionUser;
import com.vmware.identity.idm.Tenant;

/**
 * @author Yehia Zayour
 */
public class MockIdmClient extends IdmClient {
    private final String tenantName;
    private final PrivateKey tenantPrivateKey;
    private final Certificate tenantCertificate;
    private final String issuer;

    private final String clientId;
    private final String redirectUri;
    private final String logoutUri;
    private final String postLogoutRedirectUri;
    private final String clientCertSubjectDN;
    private final Certificate clientCertificate;
    private final String tokenEndpointAuthMethod;
    private final OIDCClient additionalClient;

    private final String username;
    private final String password;
    private final String gssContextId;
    private final byte[] gssServerLeg;
    private final boolean personUserEnabled;

    private final String solutionUsername;
    private final boolean solutionUserEnabled;
    private final boolean isMemberOfSystemGroup;

    private final long maxBearerTokenLifetime;
    private final long maxHoKTokenLifetime;
    private final long maxBearerRefreshTokenLifetime;
    private final long maxHoKRefreshTokenLifetime;
    private final long clockTolerance;

    private final Map<String, OIDCClient> clientMap;

    private MockIdmClient(Builder builder) {
        this.tenantName              = builder.tenantName;
        this.tenantPrivateKey        = builder.tenantPrivateKey;
        this.tenantCertificate       = builder.tenantCertificate;
        this.issuer                  = builder.issuer;

        this.clientId                = builder.clientId;
        this.redirectUri             = builder.redirectUri;
        this.logoutUri               = builder.logoutUri;
        this.postLogoutRedirectUri   = builder.postLogoutRedirectUri;
        this.clientCertSubjectDN     = builder.clientCertSubjectDN;
        this.clientCertificate       = builder.clientCertificate;
        this.tokenEndpointAuthMethod = builder.tokenEndpointAuthMethod;
        this.additionalClient        = builder.additionalClient;

        this.username                = builder.username;
        this.password                = builder.password;
        this.gssContextId            = builder.gssContextId;
        this.gssServerLeg            = builder.gssServerLeg;
        this.personUserEnabled       = builder.personUserEnabled;

        this.solutionUsername        = builder.solutionUsername;
        this.solutionUserEnabled     = builder.solutionUserEnabled;
        this.isMemberOfSystemGroup   = builder.isMemberOfSystemGroup;

        this.maxBearerTokenLifetime         = builder.maxBearerTokenLifetime;
        this.maxHoKTokenLifetime            = builder.maxHoKTokenLifetime;
        this.maxBearerRefreshTokenLifetime  = builder.maxBearerRefreshTokenLifetime;
        this.maxHoKRefreshTokenLifetime     = builder.maxHoKRefreshTokenLifetime;
        this.clockTolerance                 = builder.clockTolerance;

        this.clientMap = new HashMap<String, OIDCClient>();

        if (this.clientId != null) {
            OIDCClient client = new OIDCClient.Builder(this.clientId).
                    redirectUris(Arrays.asList(this.redirectUri)).
                    postLogoutRedirectUris(Arrays.asList(this.postLogoutRedirectUri)).
                    logoutUri(this.logoutUri).
                    certSubjectDN(this.clientCertSubjectDN).
                    tokenEndpointAuthMethod(this.tokenEndpointAuthMethod).
                    tokenEndpointAuthSigningAlg("RS256").build();
            this.clientMap.put(this.clientId, client);
        }
        if (this.additionalClient != null) {
            this.clientMap.put(this.additionalClient.getClientId(), this.additionalClient);
        }
    }

    @Override
    public PrincipalId authenticate(String tenantName, String username, String password) throws Exception {
        validateTenant(tenantName);
        Validate.notEmpty(username, "username");
        Validate.notEmpty(password, "password");

        boolean match =
                Objects.equals(username, this.username) &&
                Objects.equals(password, this.password);
        if (!match) {
            throw new IDMLoginException("invalid credentials");
        }

        return new PrincipalId(this.username, this.tenantName);
    }

    @Override
    public PrincipalId authenticate(String tenantName, X509Certificate[] tlsCertChain) throws Exception {
        throw new UnsupportedOperationException();
    }

    @Override
    public GSSResult authenticate(String tenantName, String contextId, byte[] gssTicket) throws Exception {
        validateTenant(tenantName);
        Validate.notEmpty(contextId, "contextId");
        Validate.notNull(gssTicket, "gssTicket");

        boolean match = Objects.equals(contextId, this.gssContextId);
        if (!match) {
            throw new IDMLoginException("invalid credentials");
        }

        return (this.gssServerLeg != null) ?
                new GSSResult(this.gssContextId, this.gssServerLeg) :
                new GSSResult(this.gssContextId, new PrincipalId(this.username, this.tenantName));
    }

    @Override
    public boolean isActive(String tenantName, PrincipalId id) throws Exception {
        validateTenant(tenantName);
        Validate.notNull(id, "id");

        boolean result;
        if (id.getName().equals(this.username)) {
            result = this.personUserEnabled;
        } else if (id.getName().equals(this.solutionUsername)) {
            result = this.solutionUserEnabled;
        } else {
            throw new InvalidPrincipalException("invalid principal", id.getUPN());
        }
        return result;
    }

    @Override
    public PersonUser findPersonUser(String tenantName, PrincipalId id) throws Exception {
        validateTenant(tenantName);
        Validate.notNull(id, "id");

        PersonUser result = null;
        if (id.getName().equals(this.username)) {
            PersonDetail detail = new PersonDetail.Builder().build();
            boolean disabled = !this.personUserEnabled;
            boolean locked = false;
            result = new PersonUser(id, detail, disabled, locked);
        }
        return result;
    }

    @Override
    public SolutionUser findSolutionUserByCertDn(String tenantName, String subjectDN) throws Exception {
        validateTenant(tenantName);
        Validate.notEmpty(subjectDN, "subjectDN");

        SolutionUser result = null;
        if (subjectDN.equals(this.clientCertSubjectDN)) {
            PrincipalId id = new PrincipalId(this.solutionUsername, this.tenantName);
            SolutionDetail detail = new SolutionDetail((X509Certificate) this.clientCertificate);
            boolean disabled = !this.solutionUserEnabled;
            result = new SolutionUser(id, detail, disabled);
        }
        return result;
    }

    @Override
    public Collection<AttributeValuePair> getAttributeValues(
            String tenantName,
            PrincipalId id,
            Collection<Attribute> attributes) throws Exception {
        validateTenant(tenantName);
        Validate.notNull(id, "id");
        Validate.notEmpty(attributes, "attributes");

        AttributeValuePair pair = new AttributeValuePair();
        pair.setAttrDefinition(attributes.iterator().next());
        return Arrays.asList(pair);
    }

    @Override
    public Collection<IIdentityStoreData> getProviders(
            String tenantName,
            EnumSet<DomainType> domains) throws Exception {
        validateTenant(tenantName);
        Validate.notEmpty(domains, "domains");
        return new ArrayList<IIdentityStoreData>();
    }

    @Override
    public Tenant getTenant(String tenantName) throws Exception {
        validateTenant(tenantName);
        return new Tenant(this.tenantName);
    }

    @Override
    public String getDefaultTenant() throws Exception {
        return this.tenantName;
    }

    @Override
    public PrivateKey getTenantPrivateKey(String tenantName) throws Exception {
        validateTenant(tenantName);
        return this.tenantPrivateKey;
    }

    @Override
    public List<Certificate> getTenantCertificate(String tenantName) throws Exception {
        validateTenant(tenantName);
        return Arrays.asList(this.tenantCertificate);
    }

    @Override
    public String getOIDCEntityID(String tenantName) throws Exception {
        validateTenant(tenantName);
        return this.issuer;
    }

    @Override
    public OIDCClient getOIDCClient(String tenantName, String clientId) throws Exception {
        validateTenant(tenantName);
        Validate.notEmpty(clientId, "clientId");

        OIDCClient client = this.clientMap.get(clientId);
        if (client == null) {
            throw new NoSuchOIDCClientException("client not found");
        }
        return client;
    }

    @Override
    public String getBrandName(String tenantName) throws Exception {
        validateTenant(tenantName);
        return null;
    }

    @Override
    public String getLogonBannerTitle(String tenantName) throws Exception {
        validateTenant(tenantName);
        return null;
    }

    @Override
    public String getLogonBannerContent(String tenantName) throws Exception {
        validateTenant(tenantName);
        return null;
    }

    @Override
    public boolean getLogonBannerCheckboxFlag(String tenantName) throws Exception {
        validateTenant(tenantName);
        return true;
    }

    @Override
    public long getMaximumBearerTokenLifetime(String tenantName) throws Exception {
        validateTenant(tenantName);
        return this.maxBearerTokenLifetime;
    }

    @Override
    public long getMaximumHoKTokenLifetime(String tenantName) throws Exception {
        validateTenant(tenantName);
        return this.maxHoKTokenLifetime;
    }

    @Override
    public long getMaximumBearerRefreshTokenLifetime(String tenantName) throws Exception {
        validateTenant(tenantName);
        return this.maxBearerRefreshTokenLifetime;
    }

    @Override
    public long getMaximumHoKRefreshTokenLifetime(String tenantName) throws Exception {
        validateTenant(tenantName);
        return this.maxHoKRefreshTokenLifetime;
    }

    @Override
    public long getClockTolerance(String tenantName) throws Exception {
        validateTenant(tenantName);
        return this.clockTolerance;
    }

    @Override
    public boolean isMemberOfSystemGroup(String tenantName, PrincipalId id, String groupName) throws Exception {
        validateTenant(tenantName);
        Validate.notNull(id, "id");
        Validate.notEmpty(groupName, "groupName");
        return this.isMemberOfSystemGroup;
    }

    @Override
    public String getServerSPN() {
        return null;
    }

    @Override
    public String getSsoMachineHostName() {
        return null;
    }

    private void validateTenant(String tenantName) throws NoSuchTenantException {
        Validate.notEmpty(tenantName, "tenantName");
        if (!tenantName.equals(this.tenantName)) {
            throw new NoSuchTenantException("tenant not found");
        }
    }

    public static class Builder {
        private String tenantName;
        private PrivateKey tenantPrivateKey;
        private Certificate tenantCertificate;
        private String issuer;

        private String clientId;
        private String redirectUri;
        private String logoutUri;
        private String postLogoutRedirectUri;
        private String clientCertSubjectDN;
        private Certificate clientCertificate;
        private String tokenEndpointAuthMethod;
        private OIDCClient additionalClient;

        private String username;
        private String password;
        private String gssContextId;
        private byte[] gssServerLeg;
        private boolean personUserEnabled;

        private String solutionUsername;
        private boolean solutionUserEnabled;
        private boolean isMemberOfSystemGroup;

        private long maxBearerTokenLifetime;
        private long maxHoKTokenLifetime;
        private long maxBearerRefreshTokenLifetime;
        private long maxHoKRefreshTokenLifetime;
        private long clockTolerance;

        public Builder() {
        }

        public Builder tenantName(String tenantName) {
            this.tenantName = tenantName;
            return this;
        }

        public Builder tenantPrivateKey(PrivateKey tenantPrivateKey) {
            this.tenantPrivateKey = tenantPrivateKey;
            return this;
        }

        public Builder tenantCertificate(Certificate tenantCertificate) {
            this.tenantCertificate = tenantCertificate;
            return this;
        }

        public Builder issuer(String issuer) {
            this.issuer = issuer;
            return this;
        }

        public Builder clientId(String clientId) {
            this.clientId = clientId;
            return this;
        }

        public Builder redirectUri(String redirectUri) {
            this.redirectUri = redirectUri;
            return this;
        }

        public Builder logoutUri(String logoutUri) {
            this.logoutUri = logoutUri;
            return this;
        }

        public Builder postLogoutRedirectUri(String postLogoutRedirectUri) {
            this.postLogoutRedirectUri = postLogoutRedirectUri;
            return this;
        }

        public Builder clientCertSubjectDN(String clientCertSubjectDN) {
            this.clientCertSubjectDN = clientCertSubjectDN;
            return this;
        }

        public Builder clientCertificate(Certificate clientCertificate) {
            this.clientCertificate = clientCertificate;
            return this;
        }

        public Builder tokenEndpointAuthMethod(String tokenEndpointAuthMethod) {
            Validate.notEmpty(tokenEndpointAuthMethod);
            this.tokenEndpointAuthMethod = tokenEndpointAuthMethod;
            return this;
        }

        public Builder additionalClient(OIDCClient additionalClient) {
            this.additionalClient = additionalClient;
            return this;
        }

        public Builder username(String username) {
            this.username = username;
            return this;
        }

        public Builder password(String password) {
            this.password = password;
            return this;
        }

        public Builder gssContextId(String gssContextId) {
            this.gssContextId = gssContextId;
            return this;
        }

        public Builder gssServerLeg(byte[] gssServerLeg) {
            this.gssServerLeg = gssServerLeg;
            return this;
        }

        public Builder personUserEnabled(boolean personUserEnabled) {
            this.personUserEnabled = personUserEnabled;
            return this;
        }

        public Builder solutionUsername(String solutionUsername) {
            this.solutionUsername = solutionUsername;
            return this;
        }

        public Builder solutionUserEnabled(boolean solutionUserEnabled) {
            this.solutionUserEnabled = solutionUserEnabled;
            return this;
        }

        public Builder isMemberOfSystemGroup(boolean isMemberOfSystemGroup) {
            this.isMemberOfSystemGroup = isMemberOfSystemGroup;
            return this;
        }

        public Builder maxBearerTokenLifetime(long maxBearerTokenLifetime) {
            this.maxBearerTokenLifetime = maxBearerTokenLifetime;
            return this;
        }

        public Builder maxHoKTokenLifetime(long maxHoKTokenLifetime) {
            this.maxHoKTokenLifetime = maxHoKTokenLifetime;
            return this;
        }

        public Builder maxBearerRefreshTokenLifetime(long maxBearerRefreshTokenLifetime) {
            this.maxBearerRefreshTokenLifetime = maxBearerRefreshTokenLifetime;
            return this;
        }

        public Builder maxHoKRefreshTokenLifetime(long maxHoKRefreshTokenLifetime) {
            this.maxHoKRefreshTokenLifetime = maxHoKRefreshTokenLifetime;
            return this;
        }

        public Builder clockTolerance(long clockTolerance) {
            this.clockTolerance = clockTolerance;
            return this;
        }

        public MockIdmClient build() {
            return new MockIdmClient(this);
        }
    }
}
