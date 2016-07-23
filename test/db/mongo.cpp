#include "jeho/db/req_mongo.hpp"

const std::string  db_connection_string ="mongodb://localhost:27017";
const std::string  db_name = "test";
using ptime = mongo::ptime;
struct stock_base {
  BOOST_HANA_DEFINE_STRUCT(stock_base,
			   (std::string , code),
			   (long ,start_price),
			   (long, high_price),
			   (long, low_price),
			   (long, end_price),
			   (long, trading_volume),
			   (long, trading_money),
			   (ptime, time)
			   );
};

using namespace mongo;
void insert_test()
{
  stock_base sb;
  sb.trading_volume = 1;
  sb.trading_money = 2;
  sb.start_price = 3;
  sb.time= std::chrono::system_clock::now();
  sb.code = "aaa";
  //sb.time = boost::posix_time::current_time();
  std::list<stock_base> sbs;

  sbs.push_back(sb);
  sb.code = "xxx";
  sbs.push_back(sb);

  auto con = std::make_shared<connection>(db_connection_string , db_name);
  
  //auto s = req_void<insert,stock_base>(con);

  
  insert_iterator<stock_base> it(con);
  //*it = sb;
  //it++;
  std::copy(std::begin(sbs) , std::end(sbs) , it);
}

void select_test(std::string query)
{
  jeho::db::query q(query);
  //mongo::select_iterator sel(con,q);
  //mongo::update_iterator up(con ,q);
  //mongo::remove_iterator rm(con ,q);

  auto con = std::make_shared<connection>(db_connection_string , db_name);
  auto b =  mongo::cursor<stock_base>(con, q);
  auto begin = std::begin(b);
  auto end = std::end(b);
  stock_base sb;
  std::cout<<sb.start_price<<std::endl;
  while(begin != end)
    {
      sb =*begin;
      std::cout<<"start price "<<sb.start_price<<std::endl;
      std::cout<<" code "<<sb.code<<std::endl;
      std::cout<<" time "<<mongo::to_string(sb.time)<<std::endl;
      std::cout<<"---------"<<std::endl;
      begin++;
    }

  std::cout<<"out"<<std::endl;
}


void sel_pattern(std::string query)
{
  auto con = std::make_shared<mongo::connection>(db_connection_string , db_name);
  jeho::db::query q("all");
  auto s = mongo::req_res<mongo::select,stock_base,jeho::db::query>(con,q);

  auto begin = std::begin(s);
  auto end = std::end(s);

  while(begin != end)
    {
      stock_base sb =*begin;
      std::cout<<sb.code<<std::endl;
			  begin++;
    }
}

  void in_pattern()
  {
    auto con = std::make_shared<mongo::connection>(db_connection_string, db_name);
    auto s = mongo::req_up<mongo::insert , stock_base , jeho::db::query>(con,jeho::db::query(""));
    auto it = std::begin(s);

    stock_base sb;
    sb.trading_volume = 1;
    sb.trading_money = 2;
    sb.start_price = 3;
    sb.time= std::chrono::system_clock::now();
    sb.code = "aaa";
    //sb.time = boost::posix_time::current_time();
    std::list<stock_base> sbs;

    sbs.push_back(sb);
    sb.code = "xxx";
    sbs.push_back(sb);

    std::copy(std::begin(sbs) , std::end(sbs) , it);
    
  }

int main(int argc , char* argv[])
{
  std::string s = "code = a";
  if(argc > 1)
    {
      s=std::string(argv[1]);
    }
      
  //select(s);
  //insert();
  //  sel_pattern(s);
  in_pattern();
  return 1;
}
