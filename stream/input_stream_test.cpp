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
                this->setg(base,base,base+len);//todo: here is setg(set get position)
        }

        virtual int_type underflow() //override function
        {
                printf("buffer underflow for read charactor \n");
                return EOF;
        }
};

class InputStream:public std::istream
{
public:
        InputStream(std::streambuf *b)
        :std::istream(b)
        {
        }

        ~InputStream()
        {
        }
};

int main()
{
#if 0
        std::vector<char> buffer;
        buffer.resize(32);
        std::memcpy(&buffer[0],"abc012",6);
#else
        char buffer[32]={"abc012"};
#endif

        MyStreamBuf b(&buffer[0],32); 
        InputStream os(&b);

        char c[6];
        if(!os.read(c,6))
        {
                printf("read data failed\n");
        }
        else
        {
                for(int i=0;i<6;++i)
                        printf("%c",c[i]);
        }
        
        /*
        char ch[2];
        int *val=reinterpret_cast<int *>(&ch[0]);
        *val=67;//'C'
        os.write(ch,sizeof(ch));

        for(int i=0;i<7;++i)
                printf("%c",buffer[i]);
        */

        printf("\n");
}
