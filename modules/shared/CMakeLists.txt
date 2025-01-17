# modules/shared/CMakeLists.txt

add_library(shared_lib STATIC
    src/action_decision.cpp
    src/action_order.cpp
    src/player_result.cpp

    src/message_types/from_json.cpp
    src/message_types/to_json.cpp
    src/message_types/other.cpp
    
    src/game/cards.cpp
    src/game/game_phase.cpp
    src/game/player_base.cpp
    src/game/reduced_player.cpp
    src/game/board_base.cpp
    src/game/reduced_game_state.cpp
    
    src/utils/json.cpp
    src/utils/logger.cpp
    src/utils/test_helpers.cpp
)

include_rapidjson(shared_lib)

# Add root/modules/shared/include as the public include directory
target_include_directories(shared_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/modules/shared/include
)
