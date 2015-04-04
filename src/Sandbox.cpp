#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

namespace qi  = boost::spirit::qi;
namespace phx = boost::phoenix;
namespace ascii = boost::spirit::ascii;

struct Foo
{
public:
    Foo()
    {
        std::cerr << "ctor on " << this << std::endl;
    }

    ~Foo()
    {
        std::cerr << "dtor on " << this << std::endl;
    }

    void push(int i) { 
        std::cerr << "push on " << this << std::endl;
        m_is.push_back(i);
    }

    friend std::ostream& operator<<(std::ostream& os, const Foo& f);
    
    std::vector<int> m_is;
};

void pusher(Foo* f, int i) { 
    std::cerr << "pusher on " << f << std::endl;
    f->push(i); 
}

std::ostream& operator<<(std::ostream& os, const Foo& f)
{
    os << "[";
    if( f.m_is.size() > 0 )
        os << f.m_is[0];
    for(size_t i=1; i<f.m_is.size(); ++i)
        os << "," << f.m_is[i];
    os << "]" << std::endl;
    return os;
}

template <typename Iterator>
class MyParser
{
protected:

public:
    MyParser()
    {
        mRule1 = (
            (mRule3 >> mRule5(qi::_val)) >> *mRule6(qi::_val) >> "]"
            //qi::eps[qi::_val = phx::construct<Foo*>(phx::new_<Foo>())] >>
            //(("[" >> qi::int_ >> "]")[phx::bind(&pusher, qi::_r1, qi::_1)][qi::_val = qi::_r1])(qi::_val)
        );

        mRule3 = qi::eps[qi::_val = phx::construct<Foo*>(phx::new_<Foo>())] >> "[";
        mRule5 = ( qi::int_ )[phx::bind(&Foo::push, qi::_r1, qi::_1)];
        //mRule6 = "," >> mRule5;
        mRule6 = "," >> ( qi::int_ )[phx::bind(&Foo::push, qi::_r1, qi::_1)];
/*
        mRule2 = ("[" >> qi::int_ >> "]")[qi::_val = phx::construct<Foo*>(phx::new_<Foo>(qi::_1))];

        mRule3 = qi::eps[qi::_val = phx::construct<Foo*>(phx::new_<Foo>())];

        mRule4 = ("[" >> qi::int_ >> "]")[qi::_val = phx::bind(&fooSetter1, qi::_r1, qi::_1)];

        mRule5 = ("[" >> qi::int_ >> "]")[phx::bind(&fooSetter2, qi::_r1, qi::_1)][qi::_val = qi::_r1];

        mRule6 = mRule3 >> mRule5(qi::_val);
        */
    }

    Foo* parseFooPtr(Iterator first, Iterator last)
    {
        Foo* result = NULL;
        bool r = qi::phrase_parse(
            first,
            last,
            mRule1,
            qi::space,
            result
        );
        if( !r )
        {
            std::cerr << "Stopped at:" << std::endl << std::string(first, last) << std::endl;
            return NULL;
        }
        return result;
    }

protected:
    qi::rule<Iterator, Foo*(),     qi::space_type> mRule1;
    qi::rule<Iterator, Foo*(),     qi::space_type> mRule2;
    qi::rule<Iterator, Foo*(),     qi::space_type> mRule3;
    qi::rule<Iterator, Foo*(Foo*), qi::space_type> mRule4;
    qi::rule<Iterator, void(Foo*), qi::space_type> mRule5;
    qi::rule<Iterator, void(Foo*), qi::space_type> mRule6;
};

int main(int argc, char* argv[])
{
    std::cout << "Sandbox running..." << std::endl;

    std::string fooStr1("[3,1,4,1,5,9,2]");
    MyParser<std::string::const_iterator> parser;
    Foo* fPtr = parser.parseFooPtr(fooStr1.begin(), fooStr1.end());
    if( fPtr )
        std::cout << "fPtr: " << *fPtr << std::endl;
    std::cout << "DONE" << std::endl;

    return 0;
}
