#!/bin/bash
show_help() {
    echo "Usage: $0 [OPTION]... DIRECTORY DEL_SUFFIX CTRL_SUFFIX"
    echo "Delete files with DEL_SUFFIX if a file with the same name and CTRL_SUFFIX exists."
    echo ""
    echo "Options:"
    echo "  -h, --help          display this help and exit"
}
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    show_help
    exit 0
fi
if [ $# -lt 3 ]; then
    show_help
    exit 1
fi
target_dir="$1"
del_suffix="$2"
ctrl_suffix="$3"
# Проверка: существует ли такая директория
if [ ! -d "$target_dir" ]; then
    exit 1
fi
cd "$target_dir" || exit 1
for file in *"$del_suffix"; do
    if [[ -e "$file" ]]; then
        base_name="${file%"$del_suffix"}"
        control_file="${base_name}${ctrl_suffix}"
        if [[ -f "$control_file" ]]; then
            rm "$file"
        fi
    fi
done
