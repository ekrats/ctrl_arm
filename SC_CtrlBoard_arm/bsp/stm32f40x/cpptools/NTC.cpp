#include "NTC.h"

int ntcTable[] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
	              1,  6,  12,  18,  24,  31,  37,  45,  52,  59,  67,  76,  84,  93,  102,  112,  122,  132,  142,  153,  
	             165,  176,  188,  201,  214,  227,  240,  254,  269,  284,  299,  315,  331,  348,  365,  382,  400,  418,  437,  456,  
	             476,  496,  516,  537,  558,  580,  602,  625,  648,  671,  695,  719,  744,  769,  794,  820,  846,  872,  899,  926,  
	             953,  981,  1009,  1037,  1065,  1094,  1122,  1152,  1181,  1210,  1240,  1269,  1299,  1329,  1359,  1390,  1420,  1450,  1481,  1511,  
	             1542,  1572,  1602,  1633,  1663,  1693,  1724,  1754,  1784,  1814,  1843,  1873,  1903,  1932,  1962,  1991,  2020,  2049,  2077,  2106,  
                 2133,  2161,  2188,  2216,  2244,  2271,  2297,  2323,  2349,  2374,  2400,  };

void NTC::InitBuffer()
{
    filterValue = instantaneousValue;
    sum = 0;
    for (int i = 0; i < length; i++)
    {
        buffer[i] = instantaneousValue;
        sum += instantaneousValue;
    }
}

int NTC::GetRealValue()
{
    int low = 0;
    int high = 129;//size of ntcTable - 2

    if (filterValue < 0 || filterValue > 2400)
    {
        return -41;
    }

    while (low <= high)
    {
        int middle = (low + high) / 2;
        if (filterValue >= ntcTable[middle] && filterValue < ntcTable[middle + 1])
        {
            return middle - 40;
        }
        else if (filterValue < ntcTable[middle])
        {
            high = middle - 1;
        }
        else if (filterValue > ntcTable[middle + 1])
        {
            low = middle + 1;
        }
        else
        {
            return middle + 1 - 40;
        }
    }
    return -41;
}

int NTC::GetAcqValue(int realValue)
{
    return 0;
}

void NTC::Update()
{
    int tmp = 0;
    instantaneousValue = *dataSource;
    tmp = instantaneousValue;
    //快速逼近滤波值
    if (sum == 0)
    {
        InitBuffer();
    }

    //滤波算法
    if (tmp > filterValue + filterPara)
    {
        tmp = filterValue + filterPara;
    }
    else if (tmp < filterValue - filterPara)
    {
        tmp = filterValue - filterPara;
    }
    //平均值滤波
    sum = sum + tmp - buffer[index];
    buffer[index] = tmp;

    filterValue = sum / length;
    UpdateIndex();
}
