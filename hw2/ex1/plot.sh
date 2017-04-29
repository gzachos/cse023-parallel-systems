#!/usr/bin/env bash

if [ ! -d "./plots/" ]
then
        mkdir plots
fi

gnuplot matmul.plt

