aux_source_directory(. LIB_SRCS)

# Generate the shared library from the library sources
add_library (libvabs SHARED ${LIB_SRCS})