#pragma once
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include "sentence.hpp"

namespace jeho { namespace db {

      namespace qi = boost::spirit::qi;
      namespace ascii = boost::spirit::ascii;
      namespace spirit = boost::spirit;

    struct big_op_ : qi::symbols<char, std::string>
    {
      big_op_()
      {
        add
    	  ("|"  , "|")
    	  ("&"   , "&")
    	  ;
      }

    } big_op;


    struct op_ : qi::symbols<char, std::string>
    {
      op_()
      {
        add
    	  ("<"  , "<")
    	  (">"   , ">")
	  ("<="  , "<=")
    	  (">="   , ">=")
	  ("="   , "=")
	  ("!="   , "!=")
    	  ;
      }

    }op;
    
      template<typename Iterator>
      struct query_parser : qi::grammar<Iterator, std::vector<jeho::db::sentence>(), ascii::space_type>
      {
	query_parser() : query_parser::base_type(start)
	{
	  using boost::spirit::qi::debug;
	  using ascii::char_;
	  using qi::lexeme;
	  using qi::on_error;
	  using qi::fail;
	  using qi::string;
	  using boost::phoenix::val;

	  start %= +(sub>>-big_op);
	  quoted_string %= lexeme[+(char_-' ')];
          sub %= quoted_string >> op >> quoted_string ;

	//debug(start);
	//debug(column);
	//debug(value);
	//debug(pair);
	//debug(pair_no_op);
	//debug(quoted_string);

	on_error<fail>
	    (
	     start 
	     , std::cout
	     << val("Error! Expecting ")
	     << val(" here: \"")
	     << val("\"")
	     << std::endl
	     );
              
	  //boost::report_errors();

	}

	qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
	//qi::rule<Iterator, std::string(), ascii::space_type> op;
	qi::rule<Iterator, jeho::db::column_value_map(), ascii::space_type> sub;
	qi::rule<Iterator, std::vector<jeho::db::sentence>(), ascii::space_type> start;
	//qi::rule<std::string::const_iterator, std::string(), ascii::space_type> value;
	//qi::rule<std::string::const_iterator, std::string(), ascii::space_type> quoted_string;
	//qi::rule<Iterator, column_value_map(), ascii::space_type> pair;
	//qi::rule<Iterator, column_value_map(), ascii::space_type> pair_no_op;
	//qi::rule<Iterator, column_value_map(), ascii::space_type> node;
	//qi::rule<Iterator, query_tree(), ascii::space_type> start;

      };


    std::vector<jeho::db::sentence> sentences(std::string const&str)
    {
      if(str =="" || str == "all")
	{
	  return std::vector<jeho::db::sentence>();
	}

      std::string::const_iterator iter = str.begin();
      std::string::const_iterator end = str.end();
      using boost::spirit::ascii::space;
      std::vector<jeho::db::sentence> result;
      typedef std::string::const_iterator iterator_type;
      jeho::db::query_parser<iterator_type> q;
      bool r = phrase_parse(iter, end, q, space, result);
      if(r && iter == end)
	{
	  return result;
	}

      std::string message ;
      message = "fail to parse " +str;
      std::cout<<message<<std::endl;
      throw std::runtime_error(message.c_str());
      return result;
    }
    
    }}
