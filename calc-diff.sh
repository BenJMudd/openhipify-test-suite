search_dir=translated
files=`ls $search_dir | grep .`
for entry in $files 
do
  wdiff -s  translated/$entry translated-modified/$entry | grep translated-modified/$entry
done