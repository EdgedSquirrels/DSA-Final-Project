@echo off
start cmd /k "gcc main.c -o main -O3 -std=c11 -w & g++ validator/validator.cpp -o validator/validator -O3 & main < testdata\test.in | validator\validator"