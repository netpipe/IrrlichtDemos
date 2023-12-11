set( bitcoin_client_sources bitcoin.cpp)

SET( libraries
    bitcoin_rpc_client
    ${Boost_SYSTEM_LIBRARY}
)

SETUP_LIBRARY( bitcoin_rpc_client   SOURCES   ${bitcoin_client_sources}
                            LIBRARIES ${libraries} 
                            AUTO_INSTALL_HEADERS 
                            LIBRARY_TYPE ${LIBRARY_BUILD_TYPE} )

add_executable( bitcoin_client test.cpp  )
target_link_libraries( bitcoin_client ${libraries} )

