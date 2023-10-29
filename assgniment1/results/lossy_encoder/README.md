Script used to obtain this results

```
#!/bin/bash

echo -n '' > results.txt
for i in 1 50 100 200 300 400 500 600 700 800 900 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000 11000 12000 14000 15000 16000 17000 18000 19000 20000 30000 40000 50000 60000
do
    ../sndfile-example-bin/encoder_lossy -bs $i sample.wav encoded
    out="$(ls -l encoded)"
    out=( $out )
    echo -n ${out[4]}' ' >> results.txt
done
```