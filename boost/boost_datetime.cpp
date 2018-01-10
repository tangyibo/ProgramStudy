//File: boost_datetime.cpp
//g++ boost_datetime.cpp -lboost_date_time
#include<iostream>
#include<boost/date_time/gregorian/gregorian.hpp>
#include<boost/date_time/posix_time/posix_time.hpp>
#include<ctime>

void test_date_time()
{
	boost::gregorian::date d(2017,1,4);
	std::cout<<"Year:"<<d.year()<<std::endl;
	std::cout<<"date:"<<boost::gregorian::to_simple_string(d)<<std::endl;

	boost::gregorian::date d2=boost::gregorian::from_string("2017-11-14");
	std::cout<<"Year:"<<d2.year()<<std::endl;
	std::cout<<"Week:"<<d2.week_number()<<std::endl;

	time_t now_time=time(NULL);
	struct tm *ptm=gmtime(&now_time);
	boost::gregorian::date d3=boost::gregorian::date_from_tm(*ptm);
	std::cout<<d3.day()<<std::endl;
	std::cout<<d3<<std::endl;
	std::cout<<boost::gregorian::to_iso_extended_string(d3)<<std::endl;

	boost::gregorian::date today =boost::gregorian::day_clock::local_day();
	std::cout<<"today:"<<today<<std::endl;

	boost::posix_time::ptime p1(boost::posix_time::time_from_string("2017-11-14 1:30:09"));
	std::cout<<p1<<std::endl;
}

/////////////////////////////////////////////////////////////////////////////
#include<map>
#include<string>

class WorkTimeGreek
{
	typedef std::map<boost::posix_time::time_period,std::string> greek_map_t;
private:
	greek_map_t m_maps;

public:
	WorkTimeGreek()
	{
		Init();
	}

	~WorkTimeGreek(){}

private:
	void Init()
	{
		boost::posix_time::ptime tody(boost::gregorian::day_clock::local_day());
		
		m_maps[boost::posix_time::time_period(tody,boost::posix_time::hours(9))]="befor 9:00";
		tody+=boost::posix_time::hours(9);
		
		m_maps[boost::posix_time::time_period(tody,boost::posix_time::hours(3)+boost::posix_time::minutes(3))]="9:00~12:30";
		tody+=boost::posix_time::hours(3)+boost::posix_time::minutes(3);
		
		m_maps[boost::posix_time::time_period(tody,boost::posix_time::hours(1))]="12:30~13:30";
		tody+=boost::posix_time::hours(1);
		
		m_maps[boost::posix_time::time_period(tody,boost::posix_time::hours(4)+boost::posix_time::minutes(3))]="13:30~18:00";
		tody+=boost::posix_time::hours(4)+boost::posix_time::minutes(3);
		
		m_maps[boost::posix_time::time_period(tody,boost::posix_time::hours(4))]="after 18:00";
	}

public:
	std::string Greek(const boost::posix_time::ptime &t)
	{
		greek_map_t::const_iterator it;
		for(it=m_maps.begin();it!=m_maps.end();++it)
		{
			if(it->first.contains(t))
				return it->second;
		}

		return "Unkown greek string";
	}
};


int main(int argc,char* argv[])
{
	WorkTimeGreek wg;
	std::cout<<wg.Greek(boost::posix_time::second_clock::local_time())<<std::endl;
	return 0;
}

