echo "generate data"
./gen

echo "encoding data"
time ./enc_13

echo "synthesize data"
./syn

# cat orgdata
# cat encdata
# cat syndna

echo "size of synthetic dna"
wc syndna

echo "sequencing dna"
./seq $1 $2 $3 $4 $5 $6

# cat seqdata

echo "decoding data"
time ./dec_13

# cat decdata
# cat orgdata

echo "computing hamming distance"
./eval
