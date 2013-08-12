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

import java.awt.Desktop;
import java.io.File;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.util.HashMap;
import java.util.Map;

import net.sf.jasperreports.engine.JasperRunManager;

/**
 * Aurora
 */
public class Aurora {
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Map<String, Object> parameters = new HashMap<String, Object>();
		parameters.put("GameID", Integer.valueOf(args[0]));
		parameters.put("RaceID", Integer.valueOf(args[1]));
		parameters.put("ReportTitle", args[2]);
		parameters.put("MinDate", Long.valueOf(args[3]));
		parameters.put("MaxDate", Long.valueOf(args[4]));
		try {
			File tempFile = File.createTempFile(args[5], ".pdf");
			JasperRunManager.runReportToPdfFile("jasper" + File.separator + args[5] + ".jasper", tempFile.getAbsolutePath(), parameters, getConnection());
			if (Desktop.isDesktopSupported()) {
			    try {
			        File myFile = new File(tempFile.getAbsolutePath());
			        Desktop.getDesktop().open(myFile);
			    } catch (IOException ex) {
			        // no application registered for PDFs
			    }
			}
		} catch (Exception e) {
			e.printStackTrace();
		}		
	}
	
	/**
	 * @return
	 */
	public static Connection getConnection() {
	    Connection c = null;
		try {
			Class.forName("org.sqlite.JDBC");
			c = DriverManager.getConnection("jdbc:sqlite:data\\aurora.db3");
		} catch (Exception e) {
			e.printStackTrace();
		}
		return c;
	}
	
}
