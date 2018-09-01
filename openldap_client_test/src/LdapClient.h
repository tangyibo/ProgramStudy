#ifndef LDAP_CLIENT_H
#define LDAP_CLIENT_H
extern "C"
{
#define LDAP_DEPRECATED 1  
#include <ldap.h>  
#include <lber.h>  
}
#include <string>
using std::string;

class LdapClient
{
public:
    LdapClient (const string &uri,const string &base_dn);
    virtual ~LdapClient ( );
    
    int Connect(const string &user_dn,const string &passwd);
    
    int VerifyPassword(const string &username,const string &password);
    
    static string ErrorMessage(int code)
    {
        return  ldap_err2string(code);
    }
    
private:
    int search_user_directory_entry(const string &userid,string &user_dn);
    
protected:
    LdapClient ( const LdapClient& rhs );
    LdapClient& operator=(const LdapClient& rhs );

private:
    LDAP *m_ldap;
    string m_uri;
    string m_base_dn;
};


/*
 * *****************
 *    Example:
 * *****************
 * 
 *    string ldap_uri = "ldap://192.168.10.7:389/";
 *    string user_dn = "cn=Manager,dc=wzu,dc=edu,dc=cn";
 *    string user_passwd = "secret";
 *    string base_dn = "dc=wzu,dc=edu,dc=cn";
 *
 *    string userid = "00884158";
 *    string passwd = "017624-a";
 *
 *   LdapClient client(ldap_uri, base_dn);
 *   int ret = client.Connect(user_dn, user_passwd);
 *   if (0 == ret)
 *   {
 *       int ret = client.VerifyPassword(userid, passwd);
 *      if (0 == ret)
 *          std::cout << "Auth Success!" << std::endl;
 *      else
 *          std::cout << "Error" << LdapClient::ErrorMessage(ret) << std::endl;
 *  }
 *  else
 *  {
 *      std::cout << "Connect Error:" << LdapClient::ErrorMessage(ret) << std::endl;
 *   }

*/

#endif /* LDAPCLIENT_H */

