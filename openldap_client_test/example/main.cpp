#include <stdio.h>  
#include <sys/time.h> 
extern "C"
{
#define LDAP_DEPRECATED 1  
#include <ldap.h>  
#include <lber.h>  
}

int main(int argc, char *argv[])
{
        LDAP *ld=NULL;
        LDAPMessage *result=NULL, *e=NULL;
        BerElement *ber=NULL;
        int rc=0;

        char *uri = "ldap://192.168.10.7:389/";
        char *username="cn=Manager,dc=wzu,dc=edu,dc=cn"  ;
        char *password="secret" ; 
        char *base_dn="dc=wzu,dc=edu,dc=cn"  ;

        rc = ldap_initialize(&ld, uri);
        if (ld == NULL)
        {
                perror("ldap_initialize");
                return 1;
        }

        int i_version = LDAP_VERSION3;
        struct timeval timeout;
        timeout.tv_sec = 20;
        timeout.tv_usec = 0;
        ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &i_version);
        ldap_set_option(ld, LDAP_OPT_REFERRALS, LDAP_OPT_ON);
        ldap_set_option( ld, LDAP_OPT_TIMEOUT, &timeout );
        ldap_set_option( ld, LDAP_OPT_NETWORK_TIMEOUT, (const void *)&timeout );
        if (ldap_set_option( ld, LDAP_OPT_TIMELIMIT, (void *) &timeout )
                        != LDAP_OPT_SUCCESS )
        {
                fprintf(stderr,"Could not set LDAP_OPT_TIMELIMIT %d\n", timeout );
                return( -1 ); 
        }

        rc = ldap_simple_bind_s(ld, username, password);
        if (rc != LDAP_SUCCESS)
        {
                fprintf(stderr, "ldap_simple_bind_s: rc: %d, %s\n", rc, ldap_err2string(rc));
                return 1;
        }
        printf("ldap_simple_bind_s success\n");


        rc = ldap_search_ext_s(ld, base_dn, LDAP_SCOPE_SUBTREE, "uid=00631003",
                        NULL, 0, NULL, NULL, LDAP_NO_LIMIT, LDAP_NO_LIMIT, &result);
        if (rc != LDAP_SUCCESS)
        {
                fprintf(stderr, "ldap_search_ext_s: rc: %d, %s\n", rc, ldap_err2string(rc));
                return 1;
        }
        printf("ldap_search_ext_s success\n");


        e = ldap_first_entry(ld, result);
        if (e != NULL)
        {
                char *a=NULL;
                for (a = ldap_first_attribute(ld, e, &ber); a != NULL; a = ldap_next_attribute(ld, e, ber))
                {

                        char **vals=NULL;
                        if ((vals = ldap_get_values(ld, e, a)) != NULL)
                        {
                                for (int i = 0; vals[i] != NULL; i++)
                                {
                                        printf("%s: %s\n", a, vals[i]);
                                }
                                ldap_value_free(vals);
                        }
                        ldap_memfree(a);
                }

                if (ber != NULL)
                        ber_free(ber, 0);
        }

        ldap_msgfree(result);
        ldap_unbind(ld);

        return 0;
}
