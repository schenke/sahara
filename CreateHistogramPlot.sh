echo "set terminal postscript eps enhanced color" >> next.plot
echo "set output 'Ensemble.eps'" >> next.plot

echo "set xlabel 'E [GeV]'" >> next.plot
echo "set ylabel 'P(E)'" >> next.plot

echo "set title 'Quarks bkac-to-back two pairs'" >> next.plot

echo "bin(x,w)=w*floor(x/w+0.5)" >> next.plot
echo "w=0.1" >> next.plot

echo "set xr [:4.5]" >> next.plot
echo "set yr [1e-4:5]" >> next.plot
echo "set log y" >> next.plot

STRING=""; COUNTER="1";

for DIR in "ENSEMBLET0.3" "ENSEMBLET0.4" "ENSEMBLET1.0"
do

Np=`wc -l ${DIR}/Partons.txt | awk '{print $1}'`
Nch=`wc -l ${DIR}/ChargedHadrons.txt | awk '{print $1}'`

T=`echo $DIR | sed -e "s/ENSEMBLET//"`

STRING="$STRING '${DIR}/Partons.txt' u (bin("'$4'",w)):(1.0/(w*$Np)) lt ${COUNTER} pt 4  smooth freq ti 'T=${T}',"
STRING="$STRING '${DIR}/ChargedHadrons.txt' u (bin("'$4'",w)):(1.0/(w*$Nch)) lt ${COUNTER} pt 5 smooth freq ti '',"
STRING="$STRING x*x*exp(-x/${T})/(2*${T}*${T}*${T}) lt ${COUNTER} ti '',"

COUNTER=`echo "$COUNTER +1" | bc`

done


echo "p $STRING" >> next.plot

gnuplot next.plot
rm next.plot

ps2pdf -dEPSCrop Ensemble.eps Ensemble.pdf
rm Ensemble.eps
