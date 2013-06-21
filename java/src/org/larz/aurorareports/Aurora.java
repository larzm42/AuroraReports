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
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import net.sf.jasperreports.engine.JasperRunManager;

/**
 * @author lamoor
 *
 */
public class Aurora {
	private static final DateFormat format = new SimpleDateFormat("yyyyMMddHHmmss");
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Map<String, Object> parameters = new HashMap<String, Object>();
		parameters.put("GameID", Integer.valueOf(args[0]));
		parameters.put("RaceID", Integer.valueOf(args[1]));
		parameters.put("ReportTitle", args[2]);
		try {
			String fileName = args[3] + format.format(new Date()) + ".pdf";
			JasperRunManager.runReportToPdfFile(args[3] + ".jasper", fileName, parameters, getConnection());
			if (Desktop.isDesktopSupported()) {
			    try {
			        File myFile = new File(fileName);
			        Desktop.getDesktop().open(myFile);
			    } catch (IOException ex) {
			        // no application registered for PDFs
			    }
			}
		} catch (Exception e) {
			e.printStackTrace();
		}		
	}
	
	public static Connection getConnection() {
	    Connection c = null;
		try {
			Class.forName("org.sqlite.JDBC");
			c = DriverManager.getConnection("jdbc:sqlite:C:\\Users\\lamoor\\AuroraReports-build-debug\\aurora.db3");
		} catch (Exception e) {
			e.printStackTrace();
		}
		return c;
	}
	
//	public static Connection getMDBConnection () {
//	Connection c = null;
//	try {
//		Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
//		c = DriverManager.getConnection("jdbc:odbc:Driver={Microsoft Access Driver (*.mdb)};DBQ=C:\\Users\\lamoor\\Aurora\\Stevefire.mdb;PWD=raistlin31;READONLY=true");
//	} catch (Exception e) {
//		e.printStackTrace();
//	}
//	return c;
//}

//	public static void updateMinerals(Connection mdbConn, Connection conn, Timestamp currentDate) {
//		List<Double> Duranium = new ArrayList<Double>();
//		List<Double> Neutronium = new ArrayList<Double>();
//		List<Double> Corbomite = new ArrayList<Double>();
//		List<Double> Tritanium = new ArrayList<Double>();
//		List<Double> Boronide = new ArrayList<Double>();
//		List<Double> Mercassium = new ArrayList<Double>();
//		List<Double> Vendarite = new ArrayList<Double>();
//		List<Double> Sorium = new ArrayList<Double>();
//		List<Double> Uridium = new ArrayList<Double>();
//		List<Double> Corundium = new ArrayList<Double>();
//		List<Double> Gallicite = new ArrayList<Double>();
//		List<Double> FuelStockpile = new ArrayList<Double>();
//		List<Double> Population = new ArrayList<Double>();
//		List<Integer> SystemID = new ArrayList<Integer>();
//		List<Integer> SystemBodyID = new ArrayList<Integer>();
//		Double WealthPoints = null;
//		Statement statement = null;
//		ResultSet rs = null;
//		try {
//			statement = mdbConn.createStatement();
//			rs = statement.executeQuery("SELECT \"Duranium\", \"Neutronium\", \"Corbomite\", \"Tritanium\", \"Boronide\", \"Mercassium\", \"Vendarite\", " + 
//					"\"Sorium\", \"Uridium\", \"Corundium\", \"Gallicite\", \"FuelStockpile\", \"Population\", \"SystemID\", \"SystemBodyID\" FROM Population where GameID=45 and RaceID=198");
//			while (rs.next()) {	
//				Duranium.add(rs.getDouble("Duranium"));
//				Neutronium.add(rs.getDouble("Neutronium"));
//				Corbomite.add(rs.getDouble("Corbomite"));
//				Tritanium.add(rs.getDouble("Tritanium"));
//				Boronide.add(rs.getDouble("Boronide"));
//				Mercassium.add(rs.getDouble("Mercassium"));
//				Vendarite.add(rs.getDouble("Vendarite"));
//				Sorium.add(rs.getDouble("Sorium"));
//				Uridium.add(rs.getDouble("Uridium"));
//				Corundium.add(rs.getDouble("Corundium"));
//				Gallicite.add(rs.getDouble("Gallicite"));
//				FuelStockpile.add(rs.getDouble("FuelStockpile"));
//				Population.add(rs.getDouble("Population"));
//				SystemID.add(rs.getInt("SystemID"));
//				SystemBodyID.add(rs.getInt("SystemBodyID"));
//			}
//		} catch (Exception e) {
//			e.printStackTrace();
//		} finally {
//			try {
//				rs.close();
//				statement.close();
//			} catch (SQLException e) {
//				e.printStackTrace();
//			}
//		}
//
//		try {
//			statement = mdbConn.createStatement();
//			rs = statement.executeQuery("SELECT \"WealthPoints\" FROM Race where GameID=45 and RaceID=198");
//			while (rs.next()) {	
//				WealthPoints = rs.getDouble("WealthPoints");
//			}
//		} catch (Exception e) {
//			e.printStackTrace();
//		} finally {
//			try {
//				rs.close();
//				statement.close();
//			} catch (SQLException e) {
//				e.printStackTrace();
//			}
//		}
//
//		try {
//			for (int x = 0; x < Duranium.size(); x++) {
//				PreparedStatement preparedStatement = conn.prepareStatement("INSERT into \"Minerals\" (\"Date\", \"GameID\", \"RaceID\", \"SystemID\", \"SystemBodyID\", " +
//				        "\"Duranium\", \"Neutronium\", \"Corbomite\", \"Tritanium\", \"Boronide\", \"Mercassium\", " +
//						"\"Vendarite\", \"Sorium\", \"Uridium\", \"Corundium\", \"Gallicite\") values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
//				Timestamp sqlDate = new Timestamp(currentDate.getTime());
//				preparedStatement.setTimestamp(1, sqlDate);
//				preparedStatement.setInt(2, 45);
//				preparedStatement.setInt(3, 198);
//				preparedStatement.setInt(4, SystemID.get(x));
//				preparedStatement.setInt(5, SystemBodyID.get(x));
//				preparedStatement.setDouble(6, Duranium.get(x));
//				preparedStatement.setDouble(7, Neutronium.get(x));
//				preparedStatement.setDouble(8, Corbomite.get(x));
//				preparedStatement.setDouble(9, Tritanium.get(x));
//				preparedStatement.setDouble(10, Boronide.get(x));
//				preparedStatement.setDouble(11, Mercassium.get(x));
//				preparedStatement.setDouble(12, Vendarite.get(x));
//				preparedStatement.setDouble(13, Sorium.get(x));
//				preparedStatement.setDouble(14, Uridium.get(x));
//				preparedStatement.setDouble(15, Corundium.get(x));
//				preparedStatement.setDouble(16, Gallicite.get(x));
//				
//				System.out.println("INSERT into \"Minerals\" (\"Date\" \"SystemID\", \"SystemBodyID\", " +
//				        "\"Duranium\") values (" + sqlDate + ", " + SystemID.get(x) + ", " + SystemBodyID.get(x) + ", " + Duranium.get(x) + ")");
//
//				preparedStatement.executeUpdate();
//				preparedStatement.close();
//			}
//			conn.commit();
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
//		
//		try {
//			for (int x = 0; x < FuelStockpile.size(); x++) {
//				PreparedStatement preparedStatement = conn.prepareStatement("INSERT into \"Fuel\" (\"Date\", \"GameID\", \"RaceID\", \"SystemID\", \"SystemBodyID\", " +
//				        "\"FuelStockpile\") values (?, ?, ?, ?, ?, ?)");
//				Timestamp sqlDate = new Timestamp(currentDate.getTime());
//				preparedStatement.setTimestamp(1, sqlDate);
//				preparedStatement.setInt(2, 45);
//				preparedStatement.setInt(3, 198);
//				preparedStatement.setInt(4, SystemID.get(x));
//				preparedStatement.setInt(5, SystemBodyID.get(x));
//				preparedStatement.setDouble(6, FuelStockpile.get(x));
//				System.out.println("INSERT into \"Fuel\" (\"Date\" \"SystemID\", \"SystemBodyID\", " +
//				        "\"FuelStockpile\") values (" + sqlDate + ", " + SystemID.get(x) + ", " + SystemBodyID.get(x) + ", " + FuelStockpile.get(x) + ")");
//				preparedStatement.executeUpdate();
//				preparedStatement.close();
//			}
//			conn.commit();
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
//
//		try {
//			for (int x = 0; x < Population.size(); x++) {
//				PreparedStatement preparedStatement = conn.prepareStatement("INSERT into \"Population\" (\"Date\", \"GameID\", \"RaceID\", \"SystemID\", \"SystemBodyID\", " +
//				        "\"Population\") values (?, ?, ?, ?, ?, ?)");
//				Timestamp sqlDate = new Timestamp(currentDate.getTime());
//				preparedStatement.setTimestamp(1, sqlDate);
//				preparedStatement.setInt(2, 45);
//				preparedStatement.setInt(3, 198);
//				preparedStatement.setInt(4, SystemID.get(x));
//				preparedStatement.setInt(5, SystemBodyID.get(x));
//				preparedStatement.setDouble(6, Population.get(x));
//				System.out.println("INSERT into \"Population\" (\"Date\" \"SystemID\", \"SystemBodyID\", " +
//				        "\"Population\") values (" + sqlDate + ", " + SystemID.get(x) + ", " + SystemBodyID.get(x) + ", " + Population.get(x) + ")");
//				preparedStatement.executeUpdate();
//				preparedStatement.close();
//			}
//			conn.commit();
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
//
//		try {
//			PreparedStatement preparedStatement = conn.prepareStatement("INSERT into \"Wealth\" (\"Date\", \"GameID\", \"RaceID\", \"WealthPoints\") values (?, ?, ?, ?)");
//			Timestamp sqlDate = new Timestamp(currentDate.getTime());
//			preparedStatement.setTimestamp(1, sqlDate);
//			preparedStatement.setInt(2, 45);
//			preparedStatement.setInt(3, 198);
//			preparedStatement.setDouble(4, WealthPoints);
//			System.out.println("INSERT into \"Wealth\" (\"Date\" \"WealthPoints\") values (" + sqlDate + ", " + WealthPoints + ")");
//			preparedStatement.executeUpdate();
//			preparedStatement.close();
//			conn.commit();
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
//	}
//	
//	public static boolean rowExists(Timestamp date, Connection c) {
//		Statement statement = null;
//		ResultSet rs = null;
//		try {
//			statement = c.createStatement();
//			rs = statement.executeQuery("SELECT \"Date\" FROM \"Minerals\" where \"GameID\"=45");
//			while (rs.next()) {
//				Timestamp dateInTable = rs.getTimestamp("Date");
//				if (dateInTable.getTime() == date.getTime()) {
//					return true;
//				}
//			}
//		} catch (Exception e) {
//			e.printStackTrace();
//		} finally {
//			try {
//				rs.close();
//				statement.close();
//			} catch (SQLException e) {
//				e.printStackTrace();
//			}
//		}
//		return false;
//	}
//	
//	public static Timestamp getCurrentDate(Connection c) {
//		Statement statement = null;
//		ResultSet rs = null;
//		try {
//			statement = c.createStatement();
//			rs = statement.executeQuery("SELECT \"GameTime\", \"StartYear\" FROM Game where GameID=45");
//			while (rs.next()) {			
//				int time = rs.getInt("GameTime");
//				int startYear = rs.getInt("StartYear");
//				Calendar cal = Calendar.getInstance();
//				cal.set(Calendar.MILLISECOND, 0);
//				cal.set(1970, 0, 1, 0, 0, 0);
//				cal.add(Calendar.SECOND, time);
//				cal.add(Calendar.SECOND, (startYear-1970)*12*30*24*60*60);
//				Timestamp timestamp = new Timestamp(cal.getTimeInMillis());
//				return timestamp;
//			}
//		} catch (Exception e) {
//			e.printStackTrace();
//		} finally {
//			try {
//				rs.close();
//				statement.close();
//			} catch (SQLException e) {
//				e.printStackTrace();
//			}
//		}
//		return null;
//	}
//	
//	public static String getGameName() {
//		String name = "";
//		try {
//			Statement statement = getMDBConnection().createStatement();
//			ResultSet rs = statement.executeQuery("SELECT \"Duranium\" FROM Population where GameID=45 and RaceID=198");
//			while (rs.next()) {			
//				name += rs.getDouble("Duranium") + " ";
//			}
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
//		return name;
//	}
//
}
