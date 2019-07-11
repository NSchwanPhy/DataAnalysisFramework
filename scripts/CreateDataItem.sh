if (echo $1 | grep DI); then
    echo creating Dataitem $1
    for fil in DIExample.h DIExample.cxx; do
        echo `echo $fil | sed "s/DIExample/$1/"`
        cat scripts/templates/$fil | sed "s/DIExample/$1/ig" >  `echo $fil | sed "s/DIExample/$1/"`
    done
    source scripts/makeunittest.sh $1
else
    echo "error DataItem has to start with DI, nothing created";
fi

