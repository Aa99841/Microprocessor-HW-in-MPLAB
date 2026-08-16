# The following variables contains the files used by the different stages of the build process.
set(2024final_default_default_XC8_FILE_TYPE_assemble)
set_source_files_properties(${2024final_default_default_XC8_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${2024final_default_default_XC8_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(2024final_default_default_XC8_FILE_TYPE_assemblePreprocess)
set_source_files_properties(${2024final_default_default_XC8_FILE_TYPE_assemblePreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${2024final_default_default_XC8_FILE_TYPE_assemblePreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(2024final_default_default_XC8_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../setting_hardaware/adc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../setting_hardaware/ccp1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../setting_hardaware/interrupt_manager.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../setting_hardaware/setting.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../setting_hardaware/uart.c")
set_source_files_properties(${2024final_default_default_XC8_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(2024final_default_default_XC8_FILE_TYPE_link)
set(2024final_default_image_name "default.elf")
set(2024final_default_image_base_name "default")

# The output directory of the final image.
set(2024final_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/2024final")

# The full path to the final image.
set(2024final_default_full_path_to_image ${2024final_default_output_dir}/${2024final_default_image_name})
