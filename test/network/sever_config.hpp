#pragma once

struct stock_base{
BOOST_HANA_DEFINE_STRUCT(stock_base,
			 (ptime, time),
			 (long ,start_price),
			 (long, high_price),
			 (long, low_price),
			 (long, end_price),
			 (long, trading_volume),
			 (long, trading_money),
			 );
};
