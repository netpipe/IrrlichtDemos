CPP Bitcoin RPC Client
-----------------------

This library provides a simple synchronous interface for interacting with Bitcoin's RPC server.


### Example ###

    boost::asio::io_service ios;
    bitcoin::client c( ios );
    c.connect( "localhost:8332", "user", "pass" );
    std::cerr<<"balance: "<<c.getbalance("*",0)<<std::endl;


The complete exposed API is found in bitcoin.hpp

### Donations ###

    If you find this code helpful, please send some bitcoins to 12FRNVjHoEuppFu9rJNbtxJpvzvkGN71Bn



