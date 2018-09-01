
//增加
bool COpenLDAP::AddAttrute(char *AttName,char *AttValue)
{
	LDAPMod **modifications;
	modifications = (LDAPMod **)malloc( sizeof(LDAPMod *)*1 );
	modifications[0] = (LDAPMod *)malloc( sizeof(LDAPMod) );
	modifications[0]->mod_op = LDAP_MOD_ADD; //此处一定要设置为LDAP_MOD_ADD

	//添加属性 AttName=AttValue
	modifications[0]->mod_type = "mobile";
	modifications[0]->mod_values = (char **)malloc( sizeof(char *)*2 );
	modifications[0]->mod_values[0] = "12345678911";
	modifications[0]->mod_values[1] = NULL;

	rc = ldap_modify_s( ld, "telephoneNumber=Book1,dc=maxcrc,dc=com", modifications );
	return true;
}
//删除
bool COpenLDAP::DeleteAttrute(char *AttName)
{
	LDAPMod **modifications;
	modifications = (LDAPMod **)malloc( sizeof(LDAPMod *)*1 );
	modifications[0] = (LDAPMod *)malloc( sizeof(LDAPMod) );
	modifications[0]->mod_op = LDAP_MOD_DELETE; //此处一定要设置为LDAP_MOD_DELETE

	//删除属性 AttName
	modifications[0]->mod_type = "givenName";
	modifications[0]->mod_values = (char **)malloc( sizeof(char *));
	modifications[0]->mod_values[0] = NULL;

	LDAPMod *a=modifications[0];
	rc = ldap_modify_s( ld, "telephoneNumber=Book1,dc=maxcrc,dc=com", modifications );
	return true;
}

//修改
bool COpenLDAP::ReplaceAttrute(char *AttName,char *AttValue)
{
	LDAPMod **modifications;
	modifications = (LDAPMod **)malloc( sizeof(LDAPMod *)*1 );
	modifications[0] = (LDAPMod *)malloc( sizeof(LDAPMod) );
	modifications[0]->mod_op = LDAP_MOD_REPLACE; //此处一定要设置为LDAP_MOD_ADD

	//替换属性 AttName=AttValue
	modifications[0]->mod_type ="givenName";
	modifications[0]->mod_values = (char **)malloc( sizeof(char *)*2 );
	modifications[0]->mod_values[0] = "managers";
	modifications[0]->mod_values[1] = NULL;

	rc = ldap_modify_s( ld, "telephoneNumber=Book1,dc=maxcrc,dc=com", &modifications[0] );
	return true;
}  


