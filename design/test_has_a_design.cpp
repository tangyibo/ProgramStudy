#include <iostream>
#include <string>
#include <valarray>

using namespace std;

class Student:private std::string,private std::valarray<int>
{
public:
	Student(const char* name,int *subject,int count):std::string(name),std::valarray<int>(subject,count)
	{
	}

	~Student()
	{
		std::string *name=(std::string*)this;
		name->clear();

		//std::valarray<int> *score=(std::valarray<int>*)this;
		//score->clear();
	}

	std::string name()
	{
		return (std::string)(*this);
	}

	std::valarray<int> score()
	{
		return (std::valarray<int>)(*this);
	}

	void show_scores()
	{
		const std::valarray<int> &scores=(std::valarray<int>)(*this);
		for(size_t i=0;i<scores.size();++i)
		{
			std::cout<<scores[i]<<" ";
		}
	}
};

int main(int argc,char *argv[])
{
	int scores[5]={67,87,56,75,88};
	Student stu("join",scores,5);

	std::cout<<"name:"<<stu.name()<<std::endl;
	std::cout<<"sum:"<<stu.score().sum()<<std::endl;
	std::cout<<"list:";
	stu.show_scores();
	std::cout<<std::endl;
	
	return 0;
}
