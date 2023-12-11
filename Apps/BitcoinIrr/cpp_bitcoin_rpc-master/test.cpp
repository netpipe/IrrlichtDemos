#include "bitcoin.hpp"
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>

int main( int argc, char** argv )
{
    try {
    boost::asio::io_service ios;
    bitcoin::client c( ios );
    c.connect( "localhost:8332", "user", "pass" );
    std::cerr<<"received by 12fTfVRFTYwEFfgHDf1iNkM7nVQ1gsViUm : "<<c.getreceivedbyaddress("12fTfVRFTYwEFfgHDf1iNkM7nVQ1gsViUm",0)<<std::endl;
    std::cerr<<"balance: "<<c.getbalance("*",0)<<std::endl;
    std::cerr<<"account for address: "<<c.getaccount( "12fTfVRFTYwEFfgHDf1iNkM7nVQ1gsViUm" )<<std::endl;
    std::cerr<<"address for account: "<<c.getaccountaddress( "macman5k" )<<std::endl;
    std::cerr<<"blockcount: "<<c.getblockcount()<<std::endl;
    std::cerr<<"blocknumber: "<<c.getblocknumber()<<std::endl;
    std::cerr<<"newaddress: "<<c.getnewaddress("newaddress")<<std::endl;
    bitcoin::address_info ai =c.validateaddress("12fTfVRFTYwEFfgHDf1iNkM7nVQ1gsViUm");
    std::cerr<<"validate ismine: "<<ai.ismine<<std::endl;
    std::cerr<<"validate isvalid: "<<ai.isvalid<<std::endl;
    std::cerr<<"validate account: "<<ai.account<<std::endl;
    std::cerr<<"validate address: "<<ai.address<<std::endl;
    c.setaccount("12fTfVRFTYwEFfgHDf1iNkM7nVQ1gsViUm","newname");
    c.setaccount("12fTfVRFTYwEFfgHDf1iNkM7nVQ1gsViUm","macman5k");
    c.getaddressesbyaccount("macman5k");
    } catch ( boost::exception& e )
    {
        std::cerr<< boost::diagnostic_information(e) << std::endl;
    }
}
