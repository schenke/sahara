WORKSPACE="tmp"; MULTIPLICITYCLASS="../0.95MB1.05"; T="0.2005";

for SPECIES in "Gluon"
do

mkdir ${WORKSPACE}


for EventID in `cat ${MULTIPLICITYCLASS}/EventIDList.txt`
do

r2Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $6/$2;}'`
i2Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $7/$2;}'`

r3Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $8/$2;}'`
i3Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $9/$2;}'`

r4Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $10/$2;}'`
i4Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $11/$2;}'`

tail -n+2 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk -v r2Ref=${r2Ref} -v i2Ref=${i2Ref} '{pT=$1; r2=$6/$2; i2=$7/$2; print pT,r2*r2+i2*i2,r2*r2Ref+i2*i2Ref,r2Ref*r2Ref+i2Ref*i2Ref;}' > ${WORKSPACE}/v2Delta.tmp

tail -n+2 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk -v r3Ref=${r3Ref} -v i3Ref=${i3Ref} '{pT=$1; r3=$8/$2; i3=$9/$2; print pT,r3*r3+i3*i3,r3*r3Ref+i3*i3Ref,r3Ref*r3Ref+i3Ref*i3Ref;}' > ${WORKSPACE}/v3Delta.tmp

tail -n+2 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk -v r4Ref=${r4Ref} -v i4Ref=${i4Ref} '{pT=$1; r4=$10/$2; i4=$11/$2; print pT,r4*r4+i4*i4,r4*r4Ref+i4*i4Ref,r4Ref*r4Ref+i4Ref*i4Ref;}' > ${WORKSPACE}/v4Delta.tmp

cat ${MULTIPLICITYCLASS}/EventID${EventID}/${SPECIES}SpectrumT${T}.txt | awk '{pT=$1; dNdyd2pT=$2; print pT,dNdyd2pT;}' > ${WORKSPACE}/Spectrum.tmp

for pT in `seq 0.375 0.25 18`
do

cat ${WORKSPACE}/v2Delta.tmp | awk -v pT=${pT} '{if($1==pT){print $1,$2,$3,$4,1.0;}}' >> ${WORKSPACE}/v2pT${pT}.txt
cat ${WORKSPACE}/v3Delta.tmp | awk -v pT=${pT} '{if($1==pT){print $1,$2,$3,$4,1.0;}}' >> ${WORKSPACE}/v3pT${pT}.txt
cat ${WORKSPACE}/v4Delta.tmp | awk -v pT=${pT} '{if($1==pT){print $1,$2,$3,$4,1.0;}}' >> ${WORKSPACE}/v4pT${pT}.txt
cat ${WORKSPACE}/Spectrum.tmp | awk -v pT=${pT} '{if($1==pT){print $1,$2,1.0;}}' >> ${WORKSPACE}/dN${pT}.txt


done

rm ${WORKSPACE}/v2Delta.tmp
rm ${WORKSPACE}/v3Delta.tmp
rm ${WORKSPACE}/v4Delta.tmp
rm ${WORKSPACE}/Spectrum.tmp

done


### COMPUTE STATISTICAL AVERAGE ###

for pT in `seq 0.375 0.25 18`
do

DataFile=${WORKSPACE}/v2pT${pT}.txt

NumberOfLines=`wc -l ${DataFile} | awk '{print $1;}'`

v2PP=`cat ${DataFile} | awk -v valCol=2 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v2PR=`cat ${DataFile} | awk -v valCol=3 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v2RR=`cat ${DataFile} | awk -v valCol=4 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`


echo $pT $v2PP $v2PR $v2RR >> ${MULTIPLICITYCLASS}/v2Delta${SPECIES}.txt


DataFile=${WORKSPACE}/v3pT${pT}.txt

NumberOfLines=`wc -l ${DataFile} | awk '{print $1;}'`

v3PP=`cat ${DataFile} | awk -v valCol=2 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v3PR=`cat ${DataFile} | awk -v valCol=3 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v3RR=`cat ${DataFile} | awk -v valCol=4 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`


echo $pT $v3PP $v3PR $v3RR >> ${MULTIPLICITYCLASS}/v3Delta${SPECIES}.txt


DataFile=${WORKSPACE}/v4pT${pT}.txt

NumberOfLines=`wc -l ${DataFile} | awk '{print $1;}'`

v4PP=`cat ${DataFile} | awk -v valCol=2 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v4PR=`cat ${DataFile} | awk -v valCol=3 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v4RR=`cat ${DataFile} | awk -v valCol=4 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`


echo $pT $v4PP $v4PR $v4RR >> ${MULTIPLICITYCLASS}/v4Delta${SPECIES}.txt

DataFile=${WORKSPACE}/dN${pT}.txt

NumberOfLines=`wc -l ${DataFile} | awk '{print $1;}'`

dN=`cat ${DataFile} | awk -v valCol=2 -v wCol=3 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`

echo $pT $dN >> ${MULTIPLICITYCLASS}/${SPECIES}Spectrum.txt

done

rm -r ${WORKSPACE}

done



for SPECIES in "Hadron"
do

mkdir ${WORKSPACE}


for EventID in `cat ${MULTIPLICITYCLASS}/EventIDList.txt`
do

r2Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $6/$2;}'`
i2Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $7/$2;}'`

r3Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $8/$2;}'`
i3Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $9/$2;}'`

r4Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $10/$2;}'`
i4Ref=`head -n 1 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk '{print $11/$2;}'`

tail -n+2 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk -v r2Ref=${r2Ref} -v i2Ref=${i2Ref} '{pT=$1; r2=$6/$2; i2=$7/$2; print pT,r2*r2+i2*i2,r2*r2Ref+i2*i2Ref,r2Ref*r2Ref+i2Ref*i2Ref;}' > ${WORKSPACE}/v2Delta.tmp

tail -n+2 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk -v r3Ref=${r3Ref} -v i3Ref=${i3Ref} '{pT=$1; r3=$8/$2; i3=$9/$2; print pT,r3*r3+i3*i3,r3*r3Ref+i3*i3Ref,r3Ref*r3Ref+i3Ref*i3Ref;}' > ${WORKSPACE}/v3Delta.tmp

tail -n+2 ${MULTIPLICITYCLASS}/EventID${EventID}/vN${SPECIES}T${T}.txt | awk -v r4Ref=${r4Ref} -v i4Ref=${i4Ref} '{pT=$1; r4=$10/$2; i4=$11/$2; print pT,r4*r4+i4*i4,r4*r4Ref+i4*i4Ref,r4Ref*r4Ref+i4Ref*i4Ref;}' > ${WORKSPACE}/v4Delta.tmp

cat ${MULTIPLICITYCLASS}/EventID${EventID}/${SPECIES}SpectrumT${T}.txt | awk '{pT=$1; dNdyd2pT=$2; print pT,dNdyd2pT;}' > ${WORKSPACE}/Spectrum.tmp

for pT in `seq 0.075 0.05 8`
do

cat ${WORKSPACE}/v2Delta.tmp | awk -v pT=${pT} '{if($1==pT){print $1,$2,$3,$4,1.0;}}' >> ${WORKSPACE}/v2pT${pT}.txt
cat ${WORKSPACE}/v3Delta.tmp | awk -v pT=${pT} '{if($1==pT){print $1,$2,$3,$4,1.0;}}' >> ${WORKSPACE}/v3pT${pT}.txt
cat ${WORKSPACE}/v4Delta.tmp | awk -v pT=${pT} '{if($1==pT){print $1,$2,$3,$4,1.0;}}' >> ${WORKSPACE}/v4pT${pT}.txt
cat ${WORKSPACE}/Spectrum.tmp | awk -v pT=${pT} '{if($1==pT){print $1,$2,1.0;}}' >> ${WORKSPACE}/dN${pT}.txt


done

rm ${WORKSPACE}/v2Delta.tmp
rm ${WORKSPACE}/v3Delta.tmp
rm ${WORKSPACE}/v4Delta.tmp
rm ${WORKSPACE}/Spectrum.tmp

done


### COMPUTE STATISTICAL AVERAGE ###

for pT in `seq 0.075 0.05 8`
do

DataFile=${WORKSPACE}/v2pT${pT}.txt

NumberOfLines=`wc -l ${DataFile} | awk '{print $1;}'`

v2PP=`cat ${DataFile} | awk -v valCol=2 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v2PR=`cat ${DataFile} | awk -v valCol=3 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v2RR=`cat ${DataFile} | awk -v valCol=4 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`


echo $pT $v2PP $v2PR $v2RR >> ${MULTIPLICITYCLASS}/v2Delta${SPECIES}.txt


DataFile=${WORKSPACE}/v3pT${pT}.txt

NumberOfLines=`wc -l ${DataFile} | awk '{print $1;}'`

v3PP=`cat ${DataFile} | awk -v valCol=2 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v3PR=`cat ${DataFile} | awk -v valCol=3 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v3RR=`cat ${DataFile} | awk -v valCol=4 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`


echo $pT $v3PP $v3PR $v3RR >> ${MULTIPLICITYCLASS}/v3Delta${SPECIES}.txt


DataFile=${WORKSPACE}/v4pT${pT}.txt

NumberOfLines=`wc -l ${DataFile} | awk '{print $1;}'`

v4PP=`cat ${DataFile} | awk -v valCol=2 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v4PR=`cat ${DataFile} | awk -v valCol=3 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`
v4RR=`cat ${DataFile} | awk -v valCol=4 -v wCol=5 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`


echo $pT $v4PP $v4PR $v4RR >> ${MULTIPLICITYCLASS}/v4Delta${SPECIES}.txt

DataFile=${WORKSPACE}/dN${pT}.txt

NumberOfLines=`wc -l ${DataFile} | awk '{print $1;}'`

dN=`cat ${DataFile} | awk -v valCol=2 -v wCol=3 -v NumberOfBlocks=5 -v NumberOfLines=${NumberOfLines} -f JacknifeAnalysis.awk`

echo $pT $dN >> ${MULTIPLICITYCLASS}/${SPECIES}Spectrum.txt

done

rm -r ${WORKSPACE}

done
