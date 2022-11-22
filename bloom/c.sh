#!/bin/bash
n=19

for ((i=1; i<n+1; i++))
do

(sed 's/[[:cntrl:]]$//' ./input/$i.txt | $1)>tmp
a=$(diff ./output/$i.txt tmp)
if [[ "$a" = '' ]]; then
    echo "test $i yes"
else
    echo "test $i no"
    echo ""
    echo "input:"
    cat ./input/$i.txt
    echo ""
    echo "output:"
    cat tmp
    echo ""
    echo "answer:"
    cat ./output/$i.txt
    echo ""
    echo "diff:"
    echo $a
    echo
    break
fi
done
