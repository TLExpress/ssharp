project('ssharp-cli', 'cpp')

cpp = meson.get_compiler('cpp')
cpp_std = 'c++23'

# Define the parser library
parser = static_library('ssharp-parser',
    'src/parser/pmd.cpp',
    cpp_args: ['-std=' + cpp_std],
    include_directories: include_directories('src')
)

cityhash = static_library('cityhash',
    'src/lib/cityhash/city.cpp',
    cpp_args: ['-std=' + cpp_std],
    include_directories: include_directories('src')
)

# Define the executable and link it with the parser library
executable('ssharp-cli',
    'src/tools/ssharp-cli/main.cpp',
    'src/tools/ssharp-cli/parser.cpp',
    'src/tools/ssharp-cli/hash.cpp',
    link_with: [parser, cityhash],
    cpp_args: ['-std=' + cpp_std],
    include_directories: include_directories('src')
)