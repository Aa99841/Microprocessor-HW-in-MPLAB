# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(2024final_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(2024final_default_default_XC8_FILE_TYPE_assemble)
add_library(A_2024final_default_default_XC8_assemble OBJECT ${2024final_default_default_XC8_FILE_TYPE_assemble})
    A_2024final_default_default_XC8_assemble_rule(A_2024final_default_default_XC8_assemble)
    list(APPEND 2024final_default_library_list "$<TARGET_OBJECTS:A_2024final_default_default_XC8_assemble>")

endif()

# Handle files with suffix S, for group default-XC8
if(2024final_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(A_2024final_default_default_XC8_assemblePreprocess OBJECT ${2024final_default_default_XC8_FILE_TYPE_assemblePreprocess})
    A_2024final_default_default_XC8_assemblePreprocess_rule(A_2024final_default_default_XC8_assemblePreprocess)
    list(APPEND 2024final_default_library_list "$<TARGET_OBJECTS:A_2024final_default_default_XC8_assemblePreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC8
if(2024final_default_default_XC8_FILE_TYPE_compile)
add_library(A_2024final_default_default_XC8_compile OBJECT ${2024final_default_default_XC8_FILE_TYPE_compile})
    A_2024final_default_default_XC8_compile_rule(A_2024final_default_default_XC8_compile)
    list(APPEND 2024final_default_library_list "$<TARGET_OBJECTS:A_2024final_default_default_XC8_compile>")

endif()


# Main target for this project
add_executable(2024final_default_image_EC_cUyq9 ${2024final_default_library_list})

set_target_properties(2024final_default_image_EC_cUyq9 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${2024final_default_output_dir})
set_target_properties(2024final_default_image_EC_cUyq9 PROPERTIES OUTPUT_NAME "default")
set_target_properties(2024final_default_image_EC_cUyq9 PROPERTIES SUFFIX ".elf")

target_link_libraries(2024final_default_image_EC_cUyq9 PRIVATE ${2024final_default_default_XC8_FILE_TYPE_link})


# Add the link options from the rule file.
A_2024final_default_link_rule(2024final_default_image_EC_cUyq9)



