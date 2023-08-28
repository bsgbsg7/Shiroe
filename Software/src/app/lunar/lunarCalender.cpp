// // #include "app/lunar/lunarCalender.h"
// #include <stdio.h>
// #include <stdint.h>
// #include <math.h>
// typedef struct
// {
//     uint16_t year;
//     uint8_t month;
//     uint8_t date;
//     uint8_t week;
//     uint8_t hour;
//     uint8_t minute;
//     uint8_t second;
// } Solar_t; // 公历

// typedef struct
// {
//     uint8_t has_leap_month; // 1 此年有闰月, 0 无
//     /* 如果有闰月下面2个才有意义 */
//     uint8_t leapWhichMonth;     // 此年闰月是哪个月份
//     uint8_t leapMonthis_30days; // 1 此年闰大月, 0 闰小月

//     uint8_t month;         // 当前农历月份
//     uint8_t is_leap_month; // 1 当前农历月是闰月 0非闰月
//     uint8_t date;          // 当前农历日
//     uint8_t animal;        // 此农历年生肖 1 - 12 : 鼠 - 猪
//     uint8_t tian_gan;      // 1-10甲、乙、丙、丁、戊、己、庚、辛、壬、癸
//     uint8_t di_zhi;        // 1-12子、丑、寅、卯、辰、巳、午、未、申、酉、戌、亥
// } Lunar_t;                 // 农历

// uint32_t LUNAR_INFO[] = {
//     0x04bd8, 0x04ae0, 0x0a570, 0x054d5, 0x0d260, 0x0d950, 0x16554, 0x056a0, 0x09ad0, 0x055d2, // 1900-1909
//     0x04ae0, 0x0a5b6, 0x0a4d0, 0x0d250, 0x1d255, 0x0b540, 0x0d6a0, 0x0ada2, 0x095b0, 0x14977, // 1910-1919
//     0x04970, 0x0a4b0, 0x0b4b5, 0x06a50, 0x06d40, 0x1ab54, 0x02b60, 0x09570, 0x052f2, 0x04970, // 1920-1929
//     0x06566, 0x0d4a0, 0x0ea50, 0x16a95, 0x05ad0, 0x02b60, 0x186e3, 0x092e0, 0x1c8d7, 0x0c950, // 1930-1939
//     0x0d4a0, 0x1d8a6, 0x0b550, 0x056a0, 0x1a5b4, 0x025d0, 0x092d0, 0x0d2b2, 0x0a950, 0x0b557, // 1940-1949
//     0x06ca0, 0x0b550, 0x15355, 0x04da0, 0x0a5b0, 0x14573, 0x052b0, 0x0a9a8, 0x0e950, 0x06aa0, // 1950-1959
//     0x0aea6, 0x0ab50, 0x04b60, 0x0aae4, 0x0a570, 0x05260, 0x0f263, 0x0d950, 0x05b57, 0x056a0, // 1960-1969
//     0x096d0, 0x04dd5, 0x04ad0, 0x0a4d0, 0x0d4d4, 0x0d250, 0x0d558, 0x0b540, 0x0b6a0, 0x195a6, // 1970-1979
//     0x095b0, 0x049b0, 0x0a974, 0x0a4b0, 0x0b27a, 0x06a50, 0x06d40, 0x0af46, 0x0ab60, 0x09570, // 1980-1989
//     0x04af5, 0x04970, 0x064b0, 0x074a3, 0x0ea50, 0x06b58, 0x05ac0, 0x0ab60, 0x096d5, 0x092e0, // 1990-1999
//     0x0c960, 0x0d954, 0x0d4a0, 0x0da50, 0x07552, 0x056a0, 0x0abb7, 0x025d0, 0x092d0, 0x0cab5, // 2000-2009
//     0x0a950, 0x0b4a0, 0x0baa4, 0x0ad50, 0x055d9, 0x04ba0, 0x0a5b0, 0x15176, 0x052b0, 0x0a930, // 2010-2019
//     0x07954, 0x06aa0, 0x0ad50, 0x05b52, 0x04b60, 0x0a6e6, 0x0a4e0, 0x0d260, 0x0ea65, 0x0d530, // 2020-2029
//     0x05aa0, 0x076a3, 0x096d0, 0x04afb, 0x04ad0, 0x0a4d0, 0x1d0b6, 0x0d250, 0x0d520, 0x0dd45, // 2030-2039
//     0x0b5a0, 0x056d0, 0x055b2, 0x049b0, 0x0a577, 0x0a4b0, 0x0aa50, 0x1b255, 0x06d20, 0x0ada0, // 2040-2049
//     0x14b63, 0x09370, 0x049f8, 0x04970, 0x064b0, 0x168a6, 0x0ea50, 0x06aa0, 0x1a6c4, 0x0aae0, // 2050-2059
//     0x092e0, 0x0d2e3, 0x0c960, 0x0d557, 0x0d4a0, 0x0da50, 0x05d55, 0x056a0, 0x0a6d0, 0x055d4, // 2060-2069
//     0x052d0, 0x0a9b8, 0x0a950, 0x0b4a0, 0x0b6a6, 0x0ad50, 0x055a0, 0x0aba4, 0x0a5b0, 0x052b0, // 2070-2079
//     0x0b273, 0x06930, 0x07337, 0x06aa0, 0x0ad50, 0x14b55, 0x04b60, 0x0a570, 0x054e4, 0x0d160, // 2080-2089
//     0x0e968, 0x0d520, 0x0daa0, 0x16aa6, 0x056d0, 0x04ae0, 0x0a9d4, 0x0a2d0, 0x0d150, 0x0f252, // 2090-2099
//     0x0d520                                                                                   // 2100
// };

// /**
//  * @brief  计算公历年份是闰还是平
//  * @note
//  * @param
//  * @retval
//  * @author PWH @ CSDN Tyrion.Mon
//  * @date   2021/6
//  */
// uint8_t IsLeapYear(uint16_t year)
// {
//     if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
//         return 1; // 是闰年
//     return 0;     // 是平年
// }

// /**
//  * @brief  计算公历两个日期的间隔天数
//  * @note   Solar2的年月日必须晚于Solar1或相等
//  * @param
//  * @retval
//  * @author PWH @ CSDN Tyrion.Mon
//  * @date   2021/6
//  */
// uint32_t Calc_Solar_DateInterval(Solar_t *Solar1, Solar_t *Solar2)
// {
//     int16_t i = 0;
//     uint8_t MonthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
//     uint32_t TotalDays1 = 0; // Solar1
//     uint32_t TotalDays2 = 0; // Solar2
//     uint32_t TotalDays = 0;  // 记录间隔总天数

//     /* 计算从Solar1->year 1月1日到Solar1->month Solar2->date的总天数 */
//     MonthDays[1] = (IsLeapYear(Solar1->year) ? 29 : 28);
//     for (i = 1; i < Solar1->month; i++)
//     {
//         TotalDays1 += MonthDays[i - 1]; // 累计1月到month-1月的天数
//     }
//     TotalDays1 += Solar1->date; // 最后加上month月已达天数，和值即为本年1月1日到输入日期的天数

//     /* 计算从Solar2->year 1月1日到Solar2->month Solar2->date的总天数 */
//     MonthDays[1] = (IsLeapYear(Solar2->year) ? 29 : 28);
//     for (i = 1; i < Solar2->month; i++)
//     {
//         TotalDays2 += MonthDays[i - 1];
//     }
//     TotalDays2 += Solar2->date;

//     /* 如果为同一年 */
//     if (Solar1->year == Solar2->year)
//     {
//         TotalDays = TotalDays2 - TotalDays1;
//     }
//     else
//     {
//         /* 计算两个年份之间的年份的总天数 */
//         for (i = Solar1->year + 1; i < Solar2->year; i++)
//         {
//             TotalDays += (IsLeapYear(i) ? 366 : 365);
//         }
//         /* Solar1->year剩余天数 + 两年之间年份总天数 + Solar2->year已过天数 */
//         TotalDays += ((IsLeapYear(Solar1->year) ? 366 : 365) - TotalDays1 + TotalDays2);
//     }
//     printf("TotalDays = %d\r\n", TotalDays);
//     return TotalDays;
// }

// /**
//  * @brief  计算此农历年是否有闰月
//  * @note
//  * @param
//  * @retval -1 输入错误
//  *         0 无闰月
//  *         1 有闰月
//  * @author PWH @ CSDN Tyrion.Mon
//  * @date   2021/6
//  */
// int8_t Calc_thisLunarYear_HasLeapMonth(uint16_t SolarYear, Lunar_t *Lunar)
// {
//     if (SolarYear < 1900 || SolarYear > 2100)
//         return -1;

//     /* 为 0 或者大于12， 无闰月 */
//     if ((LUNAR_INFO[SolarYear - 1900] & 0x0000000f) == 0 || (LUNAR_INFO[SolarYear - 1900] & 0x0000000f) > 12)
//     {
//         Lunar->has_leap_month = 0;
//         return 0;
//     }
//     /* 存在闰月 */
//     Lunar->has_leap_month = 1;
//     Lunar->leapWhichMonth = LUNAR_INFO[SolarYear - 1900] & 0x0000000f;             // bit3-bit0 该年闰月月份
//     Lunar->leapMonthis_30days = (LUNAR_INFO[SolarYear - 1900] >> 16) & 0x00000001; // bit16 闰月为大还是小 1大0小

//     return 1;
// }
// /**
//  * @brief  计算此年农历月（非闰月）的天数
//  * @note
//  * @param
//  * @retval -1 输入错误
//  *         正确值为29或30
//  * @author PWH @ CSDN Tyrion.Mon
//  * @date   2021/6
//  */
// int8_t Calc_thisLunarMonth_Days(uint16_t SolarYear, uint8_t LunarMonth)
// {
//     if (SolarYear < 1900 || SolarYear > 2100)
//         return -1;
//     if (LunarMonth < 1 || LunarMonth > 12)
//         return -1;
//     /* 农历1月bit15， 2月bit14，12月bit4 */
//     if ((LUNAR_INFO[SolarYear - 1900] >> ((12 - LunarMonth) + 4)) & 0x00000001)
//     {
//         return 30;
//     }
//     else
//     {
//         return 29;
//     }
// }
// /**
//  * @brief  根据间隔天数计算农历月日
//  * @note   1900/1/31为正月初一，鼠年，以此为起点
//  * @param
//  * @retval
//  * @author PWH @ CSDN Tyrion.Mon
//  * @date   2021/6
//  */
// int8_t Calc_Lunar_From_DateInterval(uint32_t DateInterval, Lunar_t *LU)
// {
//     uint16_t SolarYear = 1900;
//     uint32_t DI_temp = 0;
//     int8_t err;

//     LU->has_leap_month = 0;
//     LU->leapWhichMonth = 0;
//     LU->leapMonthis_30days = 0;
//     LU->is_leap_month = 0; // 1900农历1月非闰月
//     LU->month = 1;         // 1900正月初一始
//     LU->date = 1;
//     LU->animal = 1;          // 鼠
//     LU->tian_gan = 7;        // 庚
//     LU->di_zhi = LU->animal; // 子

//     while (1)
//     {
//         /* 查询此年是否存在闰月，闰月为闰几月，闰大月还是闰小月 */
//         err = Calc_thisLunarYear_HasLeapMonth(SolarYear, LU);
//         if (err == -1)
//             return -1; // 输入参数有误

//         /* 如果当前月为闰月 */
//         if (LU->is_leap_month == 1)
//         {
//             /* 为大月，则加30日，否则29日 */
//             DI_temp += LU->leapMonthis_30days ? 30 : 29;
//         }
//         else /* 当前月非闰月 */
//         {    /* 查询非闰月月份的天数 */
//             DI_temp += Calc_thisLunarMonth_Days(SolarYear, LU->month);
//         }
//         /* 总天数已达, 当前月即是目标农历月 */
//         if (DI_temp >= DateInterval)
//         {
//             /* 如果当前月为闰月 */
//             if (LU->is_leap_month == 1)
//             {
//                 LU->date = (LU->leapMonthis_30days ? 30 : 29) - (DI_temp - DateInterval);
//             }
//             else
//             {
//                 LU->date = Calc_thisLunarMonth_Days(SolarYear, LU->month) - (DI_temp - DateInterval);
//             }
//             return 1;
//         }

//         /* 未达 */

//         /* 此年有闰月 并且今年未到过闰月 */
//         if ((LU->has_leap_month == 1) && (LU->is_leap_month == 0))
//         {
//             /* 如果此年闰月月份和当前月一样, 此时应该在当前月和下一个月之间插入一个闰月 */
//             /* 因为上面的步骤已累加了闰月前一个月份的天数，所以直接跳到闰月，并标记当前月为闰月 */
//             if (LU->leapWhichMonth == LU->month)
//             {
//                 LU->is_leap_month = 1; // 标记现在是闰月,如果当前月是5月,匹配上了,那这个闰月就是闰5月
//                 printf("SolarYear %d LEAP MONTH is %d\r\n", SolarYear, LU->leapWhichMonth);
//             }
//             else
//             {
//                 LU->month += 1;
//             }
//         }
//         /* 此年有闰月且今个月为闰月   或者 此年无闰月 */
//         else if (((LU->has_leap_month == 1) && (LU->is_leap_month == 1)) || (LU->has_leap_month == 0))
//         {
//             LU->is_leap_month = 0; // 闰月下一个月为非闰月
//             LU->month += 1;
//         }
//         /* 溢出处理， 生肖， 天干地支 */
//         if (LU->month > 12)
//         {
//             if (++SolarYear > 2100)
//                 return -1;
//             LU->month = 1;
//             if (++LU->animal > 12)
//             {
//                 LU->animal = 1;
//             }
//             if (++LU->tian_gan > 10)
//             {
//                 LU->tian_gan = 1;
//             }
//         }
//         LU->di_zhi = LU->animal;
//     }
// }

// /**
//  * @brief  公历年月日转农历月日
//  * @note
//  * @param
//  * @retval
//  * @author PWH @ CSDN Tyrion.Mon
//  * @date   2021/6
//  */
// int8_t Solar2Lunar(Solar_t *so, Lunar_t *lu)
// {
//     Solar_t solar_1900;

//     /* 1900/1/31为正月初一，鼠年，以此为起点 */
//     solar_1900.year = 1900; /* 不 */
//     solar_1900.month = 1;   /* 要 */
//     solar_1900.date = 31;   /* 改 */
//     return Calc_Lunar_From_DateInterval(Calc_Solar_DateInterval(&solar_1900, so) + 1, lu);
// }

// /**
//  * @brief  计算某个公历年的24节气对应日期
//  * @note    2月 立春 雨水
//  *          3月 惊蛰 春分
//  *          4月 清明 谷雨
//  *          5月 立夏 小满
//  *          6月 芒种 夏至
//  *          7月 小暑 大暑
//  *          9月 白露 秋分
//  *         10月 寒露 霜降
//  *         11月 立冬 小雪
//  *         12月 大雪 冬至
//  *          1月 小寒 大寒
//  * @param
//  * @retval
//  * @author PWH @ CSDN Tyrion.Mon
//  * @date   2021/6
//  */
// int8_t Calc_24SolarTerms(uint16_t SolarYear, uint8_t *Array_24SolarTermDate)
// {
//     uint8_t i = 0;
//     uint8_t Y = SolarYear % 100; // 年份后两位
//     double D = 0.2422;
//     double C_20xx[] = // 21世纪24个节气的C值
//         {
//             3.87, 18.73, 5.63, 20.646, 4.81, 20.1,
//             5.52, 21.04, 5.678, 21.37, 7.108, 22.83,
//             7.5, 23.13, 7.646, 23.042, 8.318, 23.438,
//             7.438, 22.36, 7.18, 21.94, 5.4055, 20.12};

//     if (SolarYear < 2001 || SolarYear > 2099)
//         return -1;

//     for (i = 0; i < 24; i++)
//     {
//         if (i <= 1 || i >= 22)
//             Array_24SolarTermDate[i] = ((uint16_t)(Y * D + C_20xx[i])) - ((Y - 1) / 4);
//         else
//             Array_24SolarTermDate[i] = ((uint16_t)(Y * D + C_20xx[i])) - (Y / 4);
//         /* 例外 */
//         if (SolarYear == 2026 && i == 1)
//         {
//             Array_24SolarTermDate[i] -= 1; // 例外：2026年计算得出的雨水日期应调减一天为18日。
//         }
//         else if (SolarYear == 2084 && i == 3)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：2084年的计算结果加1日。
//         }
//         else if (SolarYear == 1911 && i == 6)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：1911年的计算结果加1日
//         }
//         else if (SolarYear == 2008 && i == 7)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：2008年的计算结果加1日
//         }
//         else if (SolarYear == 1902 && i == 8)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：1902年的计算结果加1日
//         }
//         else if (SolarYear == 1928 && i == 9)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：1928年的计算结果加1日。
//         }
//         else if ((SolarYear == 1925 || SolarYear == 2016) && i == 10)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：1925年和2016年的计算结果加1日。
//         }
//         else if (SolarYear == 1922 && i == 11)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：1922年的计算结果加1日。
//         }
//         else if (SolarYear == 2002 && i == 12)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：2002年的计算结果加1日。
//         }
//         else if (SolarYear == 1927 && i == 14)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：1927年的计算结果加1日。
//         }
//         else if (SolarYear == 1942 && i == 15)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：1942年的计算结果加1日。
//         }
//         else if (SolarYear == 2089 && i == 17)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：2089年的计算结果加1日。
//         }
//         else if (SolarYear == 2089 && i == 18)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：2089年的计算结果加1日。
//         }
//         else if (SolarYear == 1978 && i == 19)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：1978年的计算结果加1日。
//         }
//         else if (SolarYear == 1954 && i == 20)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：1954年的计算结果加1日。
//         }
//         else if ((SolarYear == 1918 || SolarYear == 2021) && i == 21)
//         {
//             Array_24SolarTermDate[i] -= 1; // 例外：1918年和2021年的计算结果减1日。
//         }
//         else if (SolarYear == 1982 && i == 22)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：1982年计算结果加1日，2019年减1日。
//         }
//         else if (SolarYear == 2019 && i == 22)
//         {
//             Array_24SolarTermDate[i] -= 1; // 例外：1982年计算结果加1日，2019年减1日。
//         }
//         else if (SolarYear == 2082 && i == 23)
//         {
//             Array_24SolarTermDate[i] += 1; // 例外：2082年的计算结果加1日
//         }
//     }

//     return 1;
// }

// int main(void)
// {
//     uint8_t i;
//     Solar_t solar_1;
//     Lunar_t Lunar_1;
//     uint8_t Array_24SoTermDate[12];

//     solar_1.year = 2023;
//     solar_1.month = 6;
//     solar_1.date = 29;

//     // Solar2Lunar(&solar_1, &Lunar_1);
//     // char tiangan[][4] = {"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"};

//     // printf("has_leap_month = %d\r\n", Lunar_1.has_leap_month);
//     // printf("leapWhichMonth = %d\r\n", Lunar_1.leapWhichMonth);
//     // printf("leapMonthis_30days = %d\r\n", Lunar_1.leapMonthis_30days);
//     // printf("month = %d\r\n", Lunar_1.month);
//     // // printf("is_leap_month = %d\r\n", Lunar_1.is_leap_month);
//     // printf("date = %d\r\n", Lunar_1.date);
//     // // printf("animal = %d\r\n", Lunar_1.animal);
//     // printf("tiangan = %d, dizhi = %d\r\n", Lunar_1.tian_gan, Lunar_1.di_zhi);

//     if (Calc_24SolarTerms(2023, Array_24SoTermDate) != -1)
//     {
//         for (i = 0; i < 24; i++)
//             printf("SolarTerm  %d: date %d\r\n", i, Array_24SoTermDate[i]);
//     }
// }
