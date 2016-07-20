
include_directories("${DEV_DIR}/src/mongo-cxx-driver/build/install/include/mongocxx/v_noabi/")
include_directories("${DEV_DIR}/src/mongo-cxx-driver/build/install/include/bsoncxx/v_noabi/")

link_directories("${DEV_DIR}/src/mongo-cxx-driver/build/install/lib")

LINK_LIBRARIES(boost_date_time)
LINK_LIBRARIES(boost_regex)
LINK_LIBRARIES(boost_thread)
LINK_LIBRARIES(mongocxx)
LINK_LIBRARIES(bsoncxx)


#add_executable(p  parser.cpp)
add_executable(m  mongo.cpp)
