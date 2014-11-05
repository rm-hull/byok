#!/usr/bin/env python

import os
import os.path
import csv


def c_files():
    for root, dirs, files in os.walk('forth/src/words'):
        for filename in files:
            if filename.endswith('.c'):
                yield os.path.join(root, filename)


def extract_entries(filename):
    with open(filename) as f:
        for line in f.readlines():
            if 'add_entry(' in line:
                yield line


if __name__ == '__main__':
    for fname in sorted(c_files()):
        print
        print "### {0}".format(fname)
        print
        print "| Word | Stack Effect | Description |"
        print "|------|--------------|-------------|"
        records = csv.reader(extract_entries(fname), skipinitialspace=True)
        for fields in sorted(records, key=lambda fields: fields[1]):
            _, word, _, stack_effect, docstring = fields

            print "| {0} | {1} | {2} |".format(word, stack_effect.replace('|', '\\|'), docstring[:-2])