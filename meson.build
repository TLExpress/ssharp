project('ssharp-cli', 'c', 'cpp')

cpp = meson.get_compiler('cpp')
cpp_std = 'c++23'

c = meson.get_compiler('c')
c_std = 'c17'

cmake = import('cmake')

zlib_ng_opts = cmake.subproject_options()
zlib_ng_opts.set_override_option('c_std', c_std)
zlib_ng_opts.add_cmake_defines({
    'BUILD_SHARED_LIBS': 'OFF',
    'ZLIB_COMPAT': 'OFF',
    'ZLIB_ENABLE_TESTS': 'OFF',
    'ZLIBNG_ENABLE_TESTS': 'OFF',
    'WITH_GTEST': 'OFF',
    'WITH_GZFILEOP': 'OFF',
})

zlib_ng_proj = cmake.subproject('zlib-ng', options: zlib_ng_opts)
zlib_ng = zlib_ng_proj.dependency('zlib')

cli11 = subproject('CLI11', default_options: ['single-file-header=false', 'precompiled=false'])
cli11_dep = cli11.get_variable('CLI11_dep')

nlohmann_json_dep = dependency('nlohmann_json', fallback: ['nlohmann_json', 'nlohmann_json_dep'])

compressor = static_library('compressor',
    'src/util/compressor.cpp',
    cpp_args: ['-std=' + cpp_std],
    include_directories: include_directories('src'),
    dependencies: [zlib_ng]
)

# Define the parser library
parser = static_library('ssharp-parser',
    'src/parser/pmd.cpp',
    'src/parser/tobj.cpp',
    'src/parser/font.cpp',
    'src/parser/sii.cpp',
    'src/parser/soundref.cpp',
    'src/parser/directory.cpp',
    cpp_args: ['-std=' + cpp_std],
    include_directories: include_directories('src')
)

cityhash = static_library('cityhash',
    'src/cityhash/city.cpp',
    cpp_args: ['-std=' + cpp_std],
    include_directories: include_directories('src')
)

# Define the executable and link it with the parser library
executable('ssharp-cli',
    'src/tools/ssharp-cli/main.cpp',
    'src/tools/ssharp-cli/ssharp-cli.hpp',
    'src/tools/ssharp-cli/parser.cpp',
    'src/tools/ssharp-cli/hash.cpp',
    'src/tools/ssharp-cli/compressor.cpp',
    link_with: [parser, cityhash, compressor],
    cpp_args: ['-std=' + cpp_std],
    include_directories: include_directories('src'),
    dependencies: [cli11_dep]
)