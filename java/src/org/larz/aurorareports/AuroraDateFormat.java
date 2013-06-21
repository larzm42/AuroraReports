/* This file is part of AuroraReports.
 *
 * AuroraReports is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dom3ai is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with AuroraReports.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.larz.aurorareports;

import java.math.BigDecimal;
import java.text.DateFormat;
import java.text.DateFormatSymbols;
import java.text.FieldPosition;
import java.text.ParsePosition;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * @author lamoor
 *
 */
public class AuroraDateFormat extends DateFormat {
	private static final long serialVersionUID = 7448222678939801179L;
	private static final BigDecimal THIRTY = BigDecimal.valueOf(30);
	private static final BigDecimal TWELVE = BigDecimal.valueOf(12);
	private static final BigDecimal secondsInDay = BigDecimal.valueOf(60*60*24);
	private static final BigDecimal secondsInMonth = BigDecimal.valueOf(60*60*24*30);
	private static final BigDecimal secondsInYear = BigDecimal.valueOf(60*60*24*30*12);
	private static final String[] months = DateFormatSymbols.getInstance().getShortMonths();

	/* (non-Javadoc)
	 * @see java.text.DateFormat#format(java.util.Date, java.lang.StringBuffer, java.text.FieldPosition)
	 */
	@Override
	public StringBuffer format(Date date, StringBuffer buffer, FieldPosition pos) {
		long seconds = date.getTime() / 1000l;
		int day = 0;
		int month = 0;
		int year = 0;
		if (seconds > 0) {
			day = BigDecimal.valueOf(seconds).divide(secondsInDay, BigDecimal.ROUND_DOWN).remainder(THIRTY).add(BigDecimal.ONE).intValue();
			month = BigDecimal.valueOf(seconds).divide(secondsInMonth, BigDecimal.ROUND_DOWN).remainder(TWELVE).add(BigDecimal.ONE).intValue();
			year = BigDecimal.valueOf(seconds).divide(secondsInYear, BigDecimal.ROUND_DOWN).add(BigDecimal.valueOf(1970)).intValue();			
		} else {
			day = BigDecimal.valueOf(seconds).divide(secondsInDay, BigDecimal.ROUND_DOWN).remainder(THIRTY).add(THIRTY).intValue();
			month = BigDecimal.valueOf(seconds).divide(secondsInMonth, BigDecimal.ROUND_DOWN).remainder(TWELVE).add(TWELVE).intValue();
			year = BigDecimal.valueOf(seconds).divide(secondsInYear, BigDecimal.ROUND_DOWN).add(BigDecimal.valueOf(1969)).intValue();			
		}
		buffer.append(day + " " + months[(int)month-1] + " " + year);
		return buffer;
	}

	/* (non-Javadoc)
	 * @see java.text.DateFormat#parse(java.lang.String, java.text.ParsePosition)
	 */
	@Override
	public Date parse(String str, ParsePosition pos) {
		SimpleDateFormat format = new SimpleDateFormat();
		return format.parse(str, pos);
	}

}
