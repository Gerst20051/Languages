How to:

    $ make
    $ PROCESSES=6   # num of CPUs
    $ for i in `seq 1 $PROCESSES`; do sleep 2; ./cracker > results$i &; done
    $ while sleep 1; do cat results* | sort -n | head -n 1; done

Now wait.
