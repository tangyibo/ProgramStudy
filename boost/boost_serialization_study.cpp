//File : boost_serialization_study.cpp
//Compile : g++ boost_serialization_study.cpp -lboost_serialization
#include <fstream>
#include <iostream>
//#include <boost/archive/text_oarchive.hpp>  
//#include <boost/archive/text_iarchive.hpp> 
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

class demo_position  
{  
private:  
        friend class boost::serialization::access;//置入式  

        template<class Archive>  
        void serialize(Archive & ar, const unsigned int version)  
        {  
               ar & degrees; //序列化或反序列化&操作符比>>和<<更为方便  
               ar & minutes;  
               ar & seconds;  
        }

private:
        int degrees;  
        int minutes;  
        float seconds;  

public:  
        demo_position(){};  
        demo_position(int d, int m, float s) :  
                degrees(d), minutes(m), seconds(s)  
        {}

        void print()
        {
                std::cout<<"degrees="<<degrees<<",minutes="<<minutes<<",seconds="<<seconds<<std::endl;
        }
}; 

int main(int argc,char *argv[])
{
        std::ofstream ofs("./filename.bin");
        demo_position g(35, 59, 24.567f);  
        g.print();

        //boost::archive::text_oarchive oa(ofs);
	boost::archive::binary_oarchive oa(ofs);  
        oa << g;
        ofs.close();

        demo_position newg;
        std::ifstream ifs("./filename.bin");

	//boost::archive::text_iarchive ia(ifs);
        boost::archive::binary_iarchive ia(ifs);  
        ia >> newg;
	ifs.close();

        newg.print();
	
        return 0;
}
