#!/usr/bin/env python3
import os
import sys
import argparse

def main():
    parser = argparse.ArgumentParser(
        description="Delete files with DEL_SUFFIX if a file with the same name and CTRL_SUFFIX exists.",
        add_help=False
    )
    group = parser.add_argument_group('Options')
    group.add_argument("-h", "--help", action="help", help="display this help and exit")
    parser.add_argument("directory", help="target directory")
    parser.add_argument("del_suffix", help="suffix of files to delete")
    parser.add_argument("ctrl_suffix", help="suffix that triggers deletion")

    # Если аргументов нет — выводим help 
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
        files = os.listdir(args.directory)
    except OSError:
        sys.exit(1)

    for filename in files:
        if filename.endswith(args.del_suffix):
            base_name = filename[:-len(args.del_suffix)]
            control_file = base_name + args.ctrl_suffix
            
            del_path = os.path.join(args.directory, filename)
            ctrl_path = os.path.join(args.directory, control_file)
            
            if os.path.isfile(ctrl_path):
                os.remove(del_path)

if __name__ == "__main__":
    main()
