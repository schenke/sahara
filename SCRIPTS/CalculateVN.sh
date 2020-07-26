for MULTIPLICITYCLASS in "../0.95MB1.05"
do

ls ${MULTIPLICITYCLASS} | sed -e "s/EventID//" -e "s/.err//" -e "s/CalcDist//" -e "s/SmearDist//" | uniq | head -n 50 > EventIDList.txt

for EventID in `cat ${MULTIPLICITYCLASS}/EventIDList.txt`
do

DIR="${MULTIPLICITYCLASS}/EventID${EventID}"


for T in "0.2005"
do

./GetVN.exe -s 0 -i ${DIR}/SpectrumT${T}fmID${EventID}.txt -ovg ${DIR}/vNGluonT${T}.txt -ovh ${DIR}/vNHadronT${T}.txt -osg ${DIR}/GluonSpectrumT${T}.txt -osh ${DIR}/HadronSpectrumT${T}.txt

./GetVN.exe -s 1 -i ${DIR}/MomentumSpaceDistributionSmearedT${T}fmID${EventID}.txt -ovg ${DIR}/vNSmearedGluonT${T}.txt -ovh ${DIR}/vNSmearedHadronT${T}.txt -osg ${DIR}/SmearedGluonSpectrumT${T}.txt -osh ${DIR}/SmearedHadronSpectrumT${T}.txt

done
done
done
