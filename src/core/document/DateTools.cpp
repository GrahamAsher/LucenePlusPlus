/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "LuceneInc.h"
#include "DateTools.h"
#include "MiscUtils.h"
#include "StringUtils.h"

namespace Lucene {

DateTools::DateOrder DateTools::dateOrder = DateTools::DATEORDER_LOCALE;

DateTools::~DateTools() { }

String DateTools::dateToString(boost_copy::ptime date, Resolution resolution)
    {
    return timeToString(MiscUtils::getTimeMillis(date), resolution);
    }

String DateTools::timeToString(int64_t time, Resolution resolution)
    {
    // Convert time in milliseconds to ISO string of form YYYYMMDDTHHMMSS, assuming time_t is time in seconds since 1/1/1970, which in practice it always is.
    std::time_t t = time / 1000000;
    std::tm tm = *gmtime(&t);
    char buffer[16] = { };
    strftime(buffer,sizeof(buffer),"%Y%m%dT%H%M%S",&tm);
    std::string timeString = buffer;

    // original code:
    // std::string timeString(boost::posix_time::to_iso_string(boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1), boost::posix_time::milliseconds(time))));
    
    switch (resolution) 
        {
		case RESOLUTION_YEAR:
			return StringUtils::toUnicode(timeString.substr(0,4).c_str());
		case RESOLUTION_MONTH:
			return StringUtils::toUnicode(timeString.substr(0,6).c_str());
		case RESOLUTION_DAY:
			return StringUtils::toUnicode(timeString.substr(0,8).c_str());
		case RESOLUTION_HOUR:
			return StringUtils::toUnicode(std::string(timeString.substr(0,8) + timeString.substr(9,2)).c_str());
		case RESOLUTION_MINUTE:
			return StringUtils::toUnicode(std::string(timeString.substr(0,8) + timeString.substr(9,4)).c_str());
		case RESOLUTION_SECOND:
			return StringUtils::toUnicode(std::string(timeString.substr(0,8) + timeString.substr(9,6)).c_str());
		case RESOLUTION_MILLISECOND:
            {
			std::string fraction(timeString.length() > 16 ? timeString.substr(16,3) : "000");
			return StringUtils::toUnicode(std::string(timeString.substr(0,8) + timeString.substr(9,6) + fraction).c_str());
			}
		case RESOLUTION_NULL:
			// silence static analyzers
			break;
		}

    throw IllegalArgumentException(L"unknown resolution '" + StringUtils::toString(resolution) + L"'");
    }

int64_t DateTools::stringToTime(const String& dateString)
    {
    return MiscUtils::getTimeMillis(stringToDate(dateString));
    }

boost_copy::ptime DateTools::stringToDate(const String& dateString)
    {
    uint16_t year = dateString.length() >= 4 ? (uint16_t)wcstol(dateString.substr(0, 4).c_str(), 0, 10) : 1970;
    uint16_t month = dateString.length() >= 6 ? (uint16_t)wcstol(dateString.substr(4, 2).c_str(), 0, 10) : 1;
    uint16_t day = dateString.length() >= 8 ? (uint16_t)wcstol(dateString.substr(6, 2).c_str(), 0, 10) : 1;
    uint16_t hour = dateString.length() >= 10 ? (uint16_t)wcstol(dateString.substr(8, 2).c_str(), 0, 10) : 0;
    uint16_t minute = dateString.length() >= 12 ? (uint16_t)wcstol(dateString.substr(10, 2).c_str(), 0, 10) : 0;
    uint16_t second = dateString.length() >= 14 ? (uint16_t)wcstol(dateString.substr(12, 2).c_str(), 0, 10) : 0;
    uint16_t millisecond = dateString.length() >= 16 ? (uint16_t)wcstol(dateString.substr(14, 3).c_str(), 0, 10) : 0;

    std::tm tm = { };
    tm.tm_year = year - 1900;
    tm.tm_mon = month;
    tm.tm_wday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    std::time_t t = mktime(&tm);
    if (t < 0)
        throw ParseException(L"Input is not valid date string: " + dateString);

    boost_copy::ptime date = double(t) + double(millisecond) / 1000.0;
    return date;
    }

boost_copy::ptime DateTools::round(const boost_copy::ptime date, Resolution resolution)
    {
    boost_copy::ptime roundDate = 0;

    std::time_t t = (std::time_t)date;
    std::tm tm = *gmtime(&t);
    std::tm tm2 = { };
    tm2.tm_mday = 1;

    switch (resolution)
        {
        case RESOLUTION_MILLISECOND:
            return date;

        case RESOLUTION_SECOND:
            tm2.tm_sec = tm.tm_sec;
            // fall through

        case RESOLUTION_MINUTE:
            tm2.tm_min = tm.tm_min;
            // fall through

        case RESOLUTION_HOUR:
            tm2.tm_hour = tm.tm_hour;
            // fall through

        case RESOLUTION_DAY:
            tm2.tm_mday = tm.tm_mday;
            // fall through

        case RESOLUTION_MONTH:
            tm2.tm_mon = tm.tm_mon;
            // fall through

        case RESOLUTION_YEAR:
            tm2.tm_year = tm.tm_year;

        case RESOLUTION_NULL:
            return 0;
        }

    t = mktime(&tm2);
    return boost_copy::ptime(t);
    }

int64_t DateTools::round(int64_t time,Resolution resolution)
    {
    boost_copy::ptime t = double(time) / 1000000.0;
    boost_copy::ptime r = round(t,resolution);
    return int64_t(r * 1000000.0);
    }

void DateTools::setDateOrder(DateTools::DateOrder order)
    {
    dateOrder = order;
    }

DateTools::DateOrder DateTools::getDateOrder(std::locale locale)
    {
    if (dateOrder != DATEORDER_LOCALE)
        return dateOrder;

    std::time_base::dateorder d = std::use_facet<std::time_get<char>>(locale).date_order();
    switch (d)
        {
        case std::time_base::no_order: default: return DATEORDER_NONE;
        case std::time_base::dmy: return DATEORDER_DMY;
        case std::time_base::mdy: return DATEORDER_MDY;
        case std::time_base::ymd: return DATEORDER_YMD;
        case std::time_base::ydm: return DATEORDER_YDM;
        }
    }

boost_copy::ptime DateTools::parseDate(const String& dateString, std::locale locale)
    {
    std::basic_istringstream<wchar_t> str(dateString);
    std::ios_base::iostate err = std::ios_base::goodbit;
    std::tm tm;
    std::istreambuf_iterator<wchar_t> ret = std::use_facet<std::time_get<wchar_t>>(locale).get_date({ str },{},str,err,&tm);
    str.setstate(err);
    if (!str)
        throw ParseException(L"Invalid date '" + dateString + L"'");

    std::time_t t = mktime(&tm);
    return boost_copy::ptime(t);
    }

}
