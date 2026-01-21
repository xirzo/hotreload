#!/usr/bin/bash

source_files=("./src/main.c")
libraries=("raylib")
program_name="hotreload"

red='\e[0;31m'
green='\e[0;32m'
no_color='\e[0m'

if [[ -z ${CC} ]]; then
    echo -e "${red}C Compiler env (CC) is not set, aborting...${no_color}"
    exit 1
else
    echo -e "${green}Found C Compiler: \"${CC}\"${no_color}"
fi

link_libraries=""

for lib in $libraries; do
    ldconfig -p | grep ${lib}

    if [[ $? -ne 0 ]]; then
        echo -e "${red}Build failed, error: could not find ${lib}${no_color}"
        exit 1
    fi

    link_libraries+="-l${lib} "
done

echo "Building sourcefiles..."


$CC ${source_files} -o ${program_name} ${link_libraries}

if [[ $? -ne  0 ]]; then
    echo -e "${red}Build failed, error code: ${?}${no_color}"
    exit 1
else
    echo -e "${green}Build ended successfully${no_color}"
fi

echo "Running the executable..."

./${program_name}
exit 0
