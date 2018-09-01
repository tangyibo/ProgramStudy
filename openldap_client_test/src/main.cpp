#include <iostream>
#include "LdapClient.h"

int main(int argc, char *argv[])
{
    string ldap_uri = "ldap://192.168.10.7:389/";
    string user_dn = "cn=Manager,dc=test,dc=edu,dc=cn";
    string user_passwd = "secret";
    string base_dn = "dc=test,dc=edu,dc=cn";

    string userid = "008ss84158";
    string passwd = "017624-a";

    LdapClient client(ldap_uri, base_dn);
    int ret = client.Connect(user_dn, user_passwd);
    if (0 == ret)
    {
        int ret = client.VerifyPassword(userid, passwd);
        if (0 == ret)
            std::cout << "Auth Success!" << std::endl;
        else
            std::cout << "Error" << LdapClient::ErrorMessage(ret) << std::endl;
    }
    else
    {
        std::cout << "Connect Error:" << LdapClient::ErrorMessage(ret) << std::endl;
    }

    return 0;
}

