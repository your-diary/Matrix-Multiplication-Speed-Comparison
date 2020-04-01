set -e

trap 'echo; echo "An error occured. Exiting..."' ERR

if [[ $# != 1 ]]; then
    echo 'No `size` is specified.'
    exit 1;
fi

num_thread=4

size=$1
echo "matrix_size: ${size}x${size}"

make > /dev/null

mkdir -p result

echo "--------------- Time in seconds ---------------"

echo -n "                          calc.out: "
./calc.out ${size} > result/result0

for num_thread in $(seq 1 ${num_thread}); do
    echo -n "calc_multithreaded.out (${num_thread} threads): "
    ./calc_multithreaded.out ${size} ${num_thread} > result/result${num_thread}
done

echo -n "                           calc.py: "
python3 calc.py ${size} > result/result$(( num_thread + 1 ))

echo "-----------------------------------------------"
echo

#Unmomment this line if you'd like to check all the outputs are exactly same.
#If `vimdiff` shows all the files in folded forms, it means everything is file.
# See `man vimdiff` for the detail.
vimdiff result/*

set +e

