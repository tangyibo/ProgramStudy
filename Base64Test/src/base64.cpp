#include "base64.h"
#include "b64/cdecode.h"
#include "b64/cencode.h"

using std::string;

namespace Base64
{

const std::string encode64( const std::string& input )
{
	string plain_input=input;
	string encode_output="";

	if(0==plain_input.size())
		return encode_output;

	base64_encodestate _state;

	base64_init_encodestate(&_state);

	char* codetext = new char[2*plain_input.size()+1];
	if(NULL==codetext)
	{
		return encode_output;
	}

	int codelength= base64_encode_block(plain_input.c_str(), plain_input.size(), codetext,&_state);
	encode_output.assign(codetext,codelength);
	codelength=base64_encode_blockend(codetext,&_state);
	encode_output.append(codetext,codelength);

	base64_init_encodestate(&_state);

	if(codetext) delete [] codetext;

	return encode_output;
}

const std::string decode64(const std::string& input )
{
#if 0
	FILE   *stream;
	char   buf[1024*1024];
        memset( buf, '\0', sizeof(buf) );
	string strCMD="echo \""+input+"\" | base64 -d";
        stream = popen( strCMD.c_str(), "r" );
	fread( buf, sizeof(char), sizeof(buf), stream);
	return string(buf);
#else
	string encode_input=input;
	string decode_output="";
	if(0==encode_input.size())
		return decode_output;

	base64_decodestate _state;
	base64_init_decodestate(&_state);

	char *decodetext=new char[encode_input.length()+1];
	if(NULL==decodetext)
		return decode_output;

	int plainlength = base64_decode_block(encode_input.c_str(), encode_input.size(), decodetext,&_state);
	decode_output.assign(decodetext,plainlength);

	base64_init_decodestate(&_state);
	
	if(decodetext) delete []decodetext;

	return decode_output;
#endif
}

}

