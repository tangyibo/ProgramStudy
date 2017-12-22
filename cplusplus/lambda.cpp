#include<algorithm>
#include<iostream>

int main()
{
	std::vector<int> vec;
	for(int i=0;i<10;++i)
	{
		vec.push_back(i);
		std::cout<<i<<" ";
	}
	
	std::cout<<std::endl;
	auto avg=[&]()
        {
                int sum=0;
                for(auto it=vec.begin();it!=vec.end();++it)
                        sum+=*it;
                return sum/vec.size();
        };
	std::cout<<"average:"<<avg()<<std::endl;

	std::transform(vec.begin(),vec.end(),vec.begin(),[](int val) ->int { return val+100;} );

	for(int i=0;i<10;++i)
		std::cout<<vec.at(i)<<" ";
	std::cout<<std::endl;


	return 0;
}
