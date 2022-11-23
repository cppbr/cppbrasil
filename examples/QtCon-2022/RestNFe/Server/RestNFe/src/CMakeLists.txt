file(GLOB_RECURSE RestNFe_SRCS *.cpp *.h)

set(RestNFe_SRCS
    ${RestNFe_SRCS}
    ${TEMPLATES_SRC}
    ../../../../nota.h
    ../../../../nota.cpp

)

# Create the application
add_library(RestNFe SHARED
    ${RestNFe_SRCS}
)

# Link to Cutelyst
target_link_libraries(RestNFe
    Cutelyst::Core
    Qt6::Core
    Qt6::Network
)

target_link_libraries(RestNFe  -lCppNFe -lcrypto -lssl -lxml2 -lz)
