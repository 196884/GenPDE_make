#!/bin/bash

PRICING_EXE=../bin/GenPDEPricer

MODEL_FILE=../resources/ASROption_01_model.xml
TRADE_FILE=../resources/ASROption_01_trade.xml

OUT_FILE=out_file

PARAM_FILE_1=nbPlanes
PARAM_NAME_1=NbValues

PARAM_FILE_2=gridSize
PARAM_NAME_2=SpaceGridSize

TMP_MODEL_FILE_1=model_1.tmp.xml
TMP_MODEL_FILE_2=model_2.tmp.xml

#RESULT=`${PRICING_EXE} -p ${TRADE_FILE} -m ${MODEL_FILE} | sed -e 's/Price: //'`
#echo "Priced (${RESULT})"

mv ${OUT_FILE} ${OUT_FILE}.bak
rm -fr ${OUT_FILE}

while read PARAM_VALUE_1
do
    sed -e "s/${PARAM_NAME_1} value=\".*\"/${PARAM_NAME_1} value=\"${PARAM_VALUE_1}\"/" ${MODEL_FILE} > ${TMP_MODEL_FILE_1}
    while read PARAM_VALUE_2
    do
        echo "Case ${PARAM_NAME_1}=${PARAM_VALUE_1}, ${PARAM_NAME_2}=${PARAM_VALUE_2}"
        sed -e "s/${PARAM_NAME_2} value=\".*\"/${PARAM_NAME_2} value=\"${PARAM_VALUE_2}\"/" ${TMP_MODEL_FILE_1} > ${TMP_MODEL_FILE_2}
        RESULT=`${PRICING_EXE} -p ${TRADE_FILE} -m ${TMP_MODEL_FILE_2} | sed -e 's/Price: //'`
        echo "${PARAM_VALUE_1}, ${PARAM_VALUE_2}, ${RESULT}," >> ${OUT_FILE}
    done <${PARAM_FILE_2}
done <${PARAM_FILE_1}

rm -fr ${TMP_MODEL_FILE_1}
