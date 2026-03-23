#!/bin/bash
show_help() {
    echo "Usage: $0 [DIRECTORY] [DEL_SUFFIX] [CTRL_SUFFIX]"
    echo "Delete files with DEL_SUFFIX if a file with the same name and CTRL_SUFFIX exists."
    echo ""
    echo "Mandatory arguments to long options are mandatory for short options too."
    echo "  -h, --help     display this help and exit"
}

if [[ "$1" == "-h" || "$1" == "--help" || $# -lt 3 ]]; then
    show_help
    exit 0
fi
DIR="$1"
DEL_SUF="$2"
CTRL_SUF="$3"
# Проверка существования директории
if [[ ! -d "$DIR" ]]; then
    exit 1
fi
cd "$DIR" || exit 1
for file in *"$DEL_SUF"; do
    [[ -e "$file" ]] || continue
    base="${file%"$DEL_SUF"}"
    ctrl_file="${base}${CTRL_SUF}"
    if [[ -f "$ctrl_file" ]]; then
        rm "$file"
    fi
done
exit 
