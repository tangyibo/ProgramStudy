//优先级队列的基本使用std::priority_queue<>
#include <cstdlib>
#include <string>
#include <iostream>
#include <queue>
using namespace std;

class ItemNode
{
    friend bool operator < (const ItemNode& lhs,const ItemNode& rhs);
private:
    int m_score;
    string m_name;
    
public:
    ItemNode(const int score,const string name)
    :m_score(score),m_name(name)
    {
        
    }
    
    void display() const
    {
        cout<<"Name:"<<m_name<<",score="<<m_score<<endl;
    }
};

bool operator < (const ItemNode& lhs,const ItemNode& rhs)
{
    return lhs.m_score<lhs.m_score;
}

int main(int argc, char** argv)
{
    priority_queue<ItemNode> ql;
    ql.push(ItemNode(1,"hello1"));
    ql.push(ItemNode(3,"hello3"));
    ql.push(ItemNode(5,"hello5"));
    ql.push(ItemNode(2,"hello2"));
    ql.push(ItemNode(4,"hello4"));
    
    while(!ql.empty())
    {
        const ItemNode& n=ql.top();
        n.display();
        ql.pop();
    }
    
    return 0;
}

