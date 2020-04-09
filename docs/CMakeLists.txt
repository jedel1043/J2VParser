set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
set(DOXYGEN_HTML_HEADER ${PROJECT_SOURCE_DIR}/extern/that_style/header.html)
set(DOXYGEN_HTML_STYLESHEET ${PROJECT_SOURCE_DIR}/extern/that_style/that_style.css)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE mainpage.md)

doxygen_add_docs(docs
        ${PROJECT_SOURCE_DIR}/include/J2VParser
        ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md
        WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}/include
        )
