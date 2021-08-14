#!bin/bash
time3=$(date "+%Y-%m-%d %H:%M:%S")
echo $time3

git add -A
git commit -m "$time3"
git push origin master
