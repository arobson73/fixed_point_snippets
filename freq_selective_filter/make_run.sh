#|/usr/bin/bash
make
if [ $? -eq 0 ]; then
    ./run_prog.sh
else
    echo -e FAIL TO BUILD
fi
