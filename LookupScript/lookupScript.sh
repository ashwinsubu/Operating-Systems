method(){
searchTerm=$1
axx=$(ls .)
for ax in $axx
  do
        if [ -f $ax ]; then
                if [[ "${ax}" =~ "${searchTerm}" ]]; then
                        echo "FILE -> " $ax
                fi
           ct=$(fgrep -c $searchTerm $ax)
           if [ $ct -gt 0 ]; then
                echo "FILE-> " $ax "CONTENT-> ... " $(fgrep -h $searchTerm $ax)
           fi
        fi
  done

  i=0
  for ax in $axx
    do
        if [ -d $ax ]; then
                i=1
                echo "DIRECTORY-> " $ax
                cd $ax
                method $searchTerm
        fi
    done
    if [ $i==0 ]; then
            cd ..
    fi
 }
 method $1
