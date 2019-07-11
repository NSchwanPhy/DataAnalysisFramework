echo creating Processor $1
for fil in ExampleProcessor.h ExampleProcessor.cxx; do
echo `echo $fil | sed "s/ExampleProcessor/$1/"`
cat scripts/templates/$fil | sed "s/ExampleProcessor/$1/ig" > `echo $fil | sed "s/ExampleProcessor/$1/"`

done
source scripts/makeunittest.sh $1
