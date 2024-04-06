search_dir=translated-modified
files=`ls $search_dir | grep .cpp`
for entry in $files 
do
  untranslated=${entry%.*}
  lineTrans=$(wc -m translated-modified/$entry | awk '{print $1}') 
  lineTransp=$(wc -m transated-pretty/$entry | awk '{print $1}') 
  lineNormal=$(wc -m original-modified/$untranslated | awk '{print $1}')
  echo $entry
  echo "Original: $lineNormal"
  echo "Translated: $lineTrans"
  echo "Pretty: $lineTransp"
  echo "Difference (orig-trans): $(($lineTrans - $lineNormal))"
  echo "Difference (orig-pretty): $(($lineTransp - $lineNormal))"
done