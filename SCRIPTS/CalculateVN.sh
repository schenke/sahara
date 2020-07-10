for MULTIPLICITYCLASS in "../0.95MB1.05"
do

ls ${MULTIPLICITYCLASS} | sed -e "s/EventID//" -e "s/.err//" -e "s/CalcDist//" -e "s/SmearDist//" | uniq > EventIDList.txt

for EventID in `cat ${MULTIPLICITYCLASS}/EventIDList.txt`
do

DIR="${MULTIPLICITYCLASS}/EventID${EventID}"

for T in "0.2005"
do

./GetVN.exe -i ${DIR}/SpectrumT${T}fmID${EventID}.txt -ovg ${DIR}/vNGluonT${T}.txt -ovh ${DIR}/vNHadronT${T}.txt -osg ${DIR}/GluonSpectrumT${T}.txt -osh ${DIR}/HadronSpectrumT${T}.txt

done
done
done
