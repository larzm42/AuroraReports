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

import net.sf.jasperreports.engine.JRChart;
import net.sf.jasperreports.engine.JRChartCustomizer;

import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.DateAxis;
import org.jfree.chart.plot.XYPlot;

/**
 * ChartCustomizer
 */
public class ChartCustomizer implements JRChartCustomizer {

	@Override
	public void customize(JFreeChart chart, JRChart arg1) {
		DateAxis xAxis = new DateAxis();	 
	    xAxis.setDateFormatOverride(new AuroraDateFormat()); 
	    ((XYPlot)chart.getPlot()).setDomainAxis(xAxis);
	}

}
