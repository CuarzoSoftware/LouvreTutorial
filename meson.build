project(
    'Louvre Example',
    'c','cpp',
    version : '0.1.0',
    meson_version: '>= 0.56.0',
    default_options: [
        'warning_level=2',
        'cpp_std=c++20',
    ]
)
 
louvre_dep = dependency('Louvre')
 
# List C/C++ source files stored in ./src
sources = run_command('find', './src', '-type', 'f', '-name', '*[.c,.cpp,.h,.hpp]', check : false).stdout().strip().split('\n')
 
executable(
    'louvre-example',
    sources : sources,
    dependencies : [
        louvre_dep,
])