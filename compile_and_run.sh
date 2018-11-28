#!/bin/bash
gcc -o pipeline main.c instruction_fetch.c instructions.c data.c
./pipeline
