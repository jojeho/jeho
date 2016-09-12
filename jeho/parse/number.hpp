#pragma once

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

namespace jeho { namespace parse {

template <typename Iterator,typename Container>
bool numbers(Iterator first, Iterator last ,Container &c)
{
  using namespace boost::spirit;
  using qi::double_;
  using qi::phrase_parse;
  using ascii::space;
  using qi::_1;
  using boost::phoenix::push_back;

  bool r = phrase_parse(
			first,                          
			last,                           
			double_[push_back(boost::phoenix::ref(c) ,_1)]
			>> *(',' >>double_[push_back(boost::phoenix::ref(c), _1)]),   
			space                           
			);
  if (first != last) // fail if we did not get a full match
    return false;
  return r;
}

  }}
