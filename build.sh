if [ "$1" = "rm" ]; then
    rm ./a.out image.png
else
    gcc ./$1 -lm
    ./a.out
fi
