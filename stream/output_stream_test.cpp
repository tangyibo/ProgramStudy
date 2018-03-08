#include <istream>
#include <vector>
#include <cstring>
#include <stdio.h>

class MyStreamBuf : public std::streambuf 
{
public:
        MyStreamBuf(char *base,const size_t len)
        :std::streambuf()
        {
                this->setbuf(base,len);
                this->setp(base,base+len);//todo: here is setp(set put position)
        }

        virtual int_type overflow(int_type ch)//override function 
        {
                printf("buffer overflow for write charactor %c\n",ch);
                return ch;
        }
};

class OutputStream:public std::ostream
{
public:
        OutputStream(std::streambuf *b)
        :std::ostream(b)
        {
        }

        ~OutputStream()
        {
        }
};

int main()
{
#if 1
        std::vector<char> buffer;
        buffer.resize(32);
#else
        char buffer[32]={0};
#endif

        MyStreamBuf b(&buffer[0],32); 
        OutputStream os(&b);
        if(!os.write("abc01",5))
        {
                printf("write data failed\n");
        }
        char ch[2];
        int *val=reinterpret_cast<int *>(&ch[0]);
        *val=67;//'C'
        os.write(ch,sizeof(ch));

        for(int i=0;i<7;++i)
                printf("%c",buffer[i]);
        printf("\n");
}
