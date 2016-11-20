#include "PT100.h"

int pt100Table[] = {209,    222,    238,    255,    269,   283,    294,   310,    325,    340,/*-40 ~ -31*/\
	355,    370,    386,    401,    415,   430,    448,   460,    477,    491,/*-30 ~ -21*/\
	505,    520,    536,    548,    564,   578,    594,   610,    622,    638,/*-20 ~ -11*/\
	650,    668,    685,    699,    714,   726,    740,   755,    772,    786,/*-10 ~ -1*/\
	800,    816,    832,    845,    860,   875,    887,   902,    917,    932,/*0   ~ 9*/\
	947,    963,    979,    991,    1006, 1027, 1036, 1050, 1065,  1078,/*10 ~ 19*/\
	1095,  1100, 1124,  1138, 1153, 1167, 1176, 1195, 1212,  1225,/*20 ~ 29*/\
	1240,  1255, 1270,  1280 ,1298, 1313, 1327, 1345, 1356,  1371,/*30 ~ 39*/\
	1382,  1400, 1414,  1429, 1443, 1458, 1472, 1484, 1458,  1515,/*40 ~ 49*/\
	1529,  1545, 1558,  1573, 1588, 1600, 1615, 1630, 1643,  1660,/*50 ~ 59*/\
	1670,  1685, 1700,  1715, 1729, 1744, 1755, 1770, 1785,  1801,/*60 ~ 69*/\
	1816,  1829, 1843,  1855, 1870, 1885, 1900, 1915, 1927,  1942,/*70 ~ 79*/\
	1958,  1972, 1987,  2000, 2013, 2028, 2041, 2059, 2072,  2085,/*80 ~ 89*/\
	2098,  2116, 2128,  2145, 2156, 2171, 2186, 2196, 2215,  2225,/*90 ~ 99*/\
	2240,  2255, 2270,  2281, 2295, 2310, 2325, 2339, 2350,  2365,/*100 ~ 109*/\
	2379,  2395, 2410,  2420, 2435, 2451, 2465, 2477, 2491,  2505,  2520,/*110 ~ 120*/\
};

void PT100::InitBuffer()
{
    filterValue = instantaneousValue;
    sum = 0;
    for (int i = 0; i < length; i++)
    {
        buffer[i] = instantaneousValue;
        sum += instantaneousValue;
    }
}

int PT100::GetRealValue()
{
    int low = 0;
    int high = 159;//size of ntcTable - 2

    if (filterValue < 209 || filterValue > 2520)
    {
        return -41;
    }

    while (low <= high)
    {
        int middle = (low + high) / 2;
        if (filterValue >= pt100Table[middle] && filterValue < pt100Table[middle + 1])
        {
            return middle - 40;
        }
        else if (filterValue < pt100Table[middle])
        {
            high = middle - 1;
        }
        else if (filterValue > pt100Table[middle + 1])
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

int PT100::GetAcqValue(int realValue)
{
    return 0;
}

void PT100::Update()
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
