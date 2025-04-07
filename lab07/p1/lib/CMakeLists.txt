aux_source_directory(. LIB_SRCS)

# Generate the shared library from the library sources
add_library (libvabs SHARED ${LIB_SRCS})

# Generate the static library from the library sources
# add_library (libvabs_static STATIC ${LIB_SRCS})