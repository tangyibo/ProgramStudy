#include "LdapClient.h"
#include <stdio.h>  
#include <stdlib.h> 
#define LOG4Z_FORMAT_INPUT_ENABLE
#include "log4z.h"

LdapClient::LdapClient(const string &uri,const string &base_dn)
:m_ldap(NULL)
,m_uri(uri)
,m_base_dn(base_dn)
{
}


LdapClient::~LdapClient()
{
    if (NULL != m_ldap)
    {
        ldap_unbind(m_ldap);
        m_ldap = NULL;
    }
    
    m_uri.clear();
}

int LdapClient::Connect(const string &user_dn,const string &passwd)
{
    int rc=0;
    
    rc=ldap_initialize(&m_ldap, m_uri.c_str());
    if(LDAP_SUCCESS!=rc || NULL == m_ldap)
    {
         LOGFMT_ERROR(LOG4Z_MAIN_LOGGER_ID,"ldap_initialize(): rc: %d, %s\n", rc, ldap_err2string(rc));
        return rc;
    }

    int i_version = LDAP_VERSION3;

    struct timeval timeout;
    timeout.tv_sec = 20;
    timeout.tv_usec = 0;

    ldap_set_option(m_ldap, LDAP_OPT_PROTOCOL_VERSION, &i_version);
    ldap_set_option(m_ldap, LDAP_OPT_REFERRALS, LDAP_OPT_ON);
    ldap_set_option(m_ldap, LDAP_OPT_TIMEOUT, &timeout);
    ldap_set_option(m_ldap, LDAP_OPT_NETWORK_TIMEOUT, (const void *) &timeout);
    if (ldap_set_option(m_ldap, LDAP_OPT_TIMELIMIT, (void *) &timeout) != LDAP_OPT_SUCCESS)
    {
        LOGFMT_ERROR(LOG4Z_MAIN_LOGGER_ID,"Could not set LDAP_OPT_TIMELIMIT %d\n", (int) timeout.tv_sec);
        return -1;
    }

    LOGFMT_TRACE(LOG4Z_MAIN_LOGGER_ID,"USER-DN:%s,PASSWORD:%s",user_dn.c_str(), passwd.c_str());
    rc = ldap_simple_bind_s(m_ldap, user_dn.c_str(), passwd.c_str());
    if (rc != LDAP_SUCCESS)
    {
        LOGFMT_ERROR(LOG4Z_MAIN_LOGGER_ID,"ldap_simple_bind_s: rc: %d, %s\n", rc, ldap_err2string(rc));
        return rc;
    }
    
    return 0;
}

int LdapClient::VerifyPassword(const string &username,const string &password)
{
    string user_dn="";
    int rc=search_user_directory_entry(username,user_dn);
    if(0==rc && !user_dn.empty())
    {
        rc = ldap_simple_bind_s(m_ldap, user_dn.c_str(), password.c_str());
        if (rc == LDAP_SUCCESS)
        {
            return 0;//success
        }
        else
        {
            return rc;//invalid password
        }
    }
   
    return rc;
}

int LdapClient::search_user_directory_entry(const string &userid,string &user_dn)
{
    int rc = -1;
    user_dn="";
    string filter = "uid=" + userid;

    if (NULL == m_ldap)
    {
        return -1;
    }

    LDAPMessage *result = NULL;
    rc = ldap_search_ext_s(m_ldap, m_base_dn.c_str(), LDAP_SCOPE_SUBTREE, filter.c_str(),
            NULL, 0, NULL, NULL, LDAP_NO_LIMIT, LDAP_NO_LIMIT, &result);
    if (rc != LDAP_SUCCESS)
    {
        LOGFMT_ERROR(LOG4Z_MAIN_LOGGER_ID,"ldap_search_ext_s(): rc: %d, %s\n", rc, ldap_err2string(rc));
        return rc;
    }

    if (NULL != result)
    {
        LDAPMessage *entry = ldap_first_entry(m_ldap, result);
        if (entry != NULL)
        {
            char* dn = ldap_get_dn(m_ldap, entry);
            if (dn != NULL)
            {
                user_dn.assign(dn);
                ldap_memfree(dn);
                rc=0;
            }
            else
            {
                rc=-1;
            }
        }
        else
        {
            rc=-1;
        }

        ldap_msgfree(result);
        result = NULL;
    }
    
    return rc;
}
