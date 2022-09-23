find_program(CASTXML_PATH castxml REQUIRED)

function(cast_xml target_name input_header output_filename)
    add_custom_command(OUTPUT "${output_filename}"
    COMMAND ${CASTXML_PATH} 
    "--castxml-gccxml" 
    "${input_header}"
    -o "${output_filename}"
    BYPRODUCTS "${output_filename}")
    if (NOT TARGET ${target_name}_xml)
        add_custom_target(${target_name}_xml
            ALL 
            DEPENDS "${output_filename}")
    else()
        set_property(TARGET ${target_name}_xml APPEND
            PROPERTY DEPENDS "${output_filename}")
    endif()
endfunction()