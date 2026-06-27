if [ "$#" = 1 ]; then
    if [ "$1" = "rm" ]; then
        rm ./a.out image.png
    else
        gcc ./$1 -lm
        ./a.out
    fi
elif [ "$#" = 2 ]; then
    if [ "$1" = "W" ]; then
        gcc -Wall -Wextra -Wconversion -pedantic ./$2 -lm
    fi
elif [ "$#" = 0 ]; then
    echo "Nothing"
fi
