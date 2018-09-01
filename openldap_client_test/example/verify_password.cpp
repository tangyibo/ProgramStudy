#include <stdio.h>  
#include <sys/time.h> 
extern "C"
{
#define LDAP_DEPRECATED 1  
#include <ldap.h>  
#include <lber.h>  
}

#include <string>
using std::string;


int verify_user_password(string userid,string userpasswd)
{
        LDAP *ld=NULL;
        LDAPMessage *result=NULL, *e=NULL;
        BerElement *ber=NULL;
        int rc=0;

        char *uri = "ldap://192.168.10.7:389/";
        char *base_user="cn=Manager,dc=wzu,dc=edu,dc=cn"  ;
        char *base_pass="secret" ; 
        char *base_dn="dc=wzu,dc=edu,dc=cn"  ;

        rc = ldap_initialize(&ld, uri);
        if (ld == NULL)
        {
                perror("ldap_initialize");
                return -1;
        }

        int i_version = LDAP_VERSION3;
        struct timeval timeout;
        timeout.tv_sec = 20;
        timeout.tv_usec = 0;
        ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &i_version);
        ldap_set_option(ld, LDAP_OPT_REFERRALS, LDAP_OPT_ON);
        ldap_set_option(ld, LDAP_OPT_TIMEOUT, &timeout );
        ldap_set_option(ld, LDAP_OPT_NETWORK_TIMEOUT, (const void *)&timeout );
        if (ldap_set_option( ld, LDAP_OPT_TIMELIMIT, (void *) &timeout )
                        != LDAP_OPT_SUCCESS )
        {
                fprintf(stderr,"Could not set LDAP_OPT_TIMELIMIT %d\n",(int) timeout.tv_sec );
                return -1; 
        }

        rc = ldap_simple_bind_s(ld,base_user,base_pass);
        if (rc != LDAP_SUCCESS)
        {
                fprintf(stderr, "ldap_simple_bind_s: rc: %d, %s\n", rc, ldap_err2string(rc));
                return 1;
        }

        string filter="uid="+userid;
        rc = ldap_search_ext_s(ld, base_dn, LDAP_SCOPE_SUBTREE, filter.c_str(),
                        NULL, 0, NULL, NULL, LDAP_NO_LIMIT, LDAP_NO_LIMIT, &result);
        if (rc != LDAP_SUCCESS)
        {
                fprintf(stderr, "ldap_search_ext_s: rc: %d, %s\n", rc, ldap_err2string(rc));
                return -1;
        }

        e = ldap_first_entry(ld, result);
        if (e != NULL)
        {
                char* dn = ldap_get_dn(ld,e);
                if(dn != NULL)
                {
                        printf("Returned DN: %s\n",dn);
                }

                //DN:uid=00884158,ou=tempteacher,ou=people,dc=wzu,dc=edu,dc=cn
                rc = ldap_simple_bind_s(ld, dn,userpasswd.c_str());
                if (rc != LDAP_SUCCESS)
                {
                        fprintf(stderr, "auth failed: rc: %d, %s\n", rc, ldap_err2string(rc));
                }
                else
                {
                        printf("auth success\n");
                }

                if(dn!=NULL)
                {       
                        ldap_memfree(dn);
                }

                /*
                for (char *attr = ldap_first_attribute(ld, e, &ber); attr != NULL; attr = ldap_next_attribute(ld, e, ber))
                {
                        char **vals=ldap_get_values(ld, e, attr);
                        if (vals != NULL)
                        {
                                printf("%s: %s\n", attr, *vals);
                                ldap_value_free(vals);
                        }

                        ldap_memfree(attr);
                }
                */

                if (ber != NULL)
                {
                        ber_free(ber, 0);
                        ber=NULL;
                }
        }

        ldap_msgfree(result);
        ldap_unbind(ld);

        return rc;
}



int main(int argc, char *argv[])
{
        string userid="00884158";
        string passwd="017624-a";

        int ret=verify_user_password(userid,passwd);

        return 0;
}

