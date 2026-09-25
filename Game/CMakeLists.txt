add_executable(Strato
    src/main.cpp

    src/Aircraft/Aircraft.cpp
)

target_link_libraries(Strato
    PRIVATE
        Wrangler
)