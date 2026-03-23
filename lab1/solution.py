#!/usr/bin/env python3
import os
import sys
import argparse

def main():
    parser = argparse.ArgumentParser(
        description="Удаление файлов с заданным суффиксом при наличии контрольного файла.",
        formatter_class=argparse.RawTextHelpFormatter,
        add_help=False
    )
    options = parser.add_argument_group('Options')
    options.add_argument("-h", "--help", action="help", help="show this help message and exit")

    parser.add_argument("directory", help="Целевая директория для сканирования")
    parser.add_argument("del_suffix", help="Суффикс файлов, которые нужно удалить (например, .bak)")
    parser.add_argument("ctrl_suffix", help="Контрольный суффикс (например, .txt)")

    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(0)

    try:
        args = parser.parse_args()
    except SystemExit:
        sys.exit(1)

    if not os.path.isdir(args.directory):
        sys.exit(1)

    try:
        items = os.listdir(args.directory)
    except OSError:
        sys.exit(1)

    # Основной цикл обработки
    for item in items:
        if item.endswith(args.del_suffix):
            base_name = item[:-len(args.del_suffix)]
            control_item = base_name + args.ctrl_suffix
            del_path = os.path.join(args.directory, item)
            ctrl_path = os.path.join(args.directory, control_item)
            if os.path.isfile(ctrl_path):
                try:
                    os.remove(del_path)
                except OSError:
                    continue

if __name__ == "__main__":
    main()
