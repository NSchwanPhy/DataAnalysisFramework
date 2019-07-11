#!/bin/bash

#for i in `ls test.txt`; do
for i in `ls *.*`; do
    mv $i tobechanged 
    cat tobechanged | sed "s/BetaFramework/ExampleFramework/g" > $i
    echo updated $i
    diff tobechanged $i
    rm tobechanged
done
