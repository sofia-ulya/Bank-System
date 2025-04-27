-- phpMyAdmin SQL Dump
-- version 4.9.2
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1:3306
-- Generation Time: Jun 28, 2022 at 12:51 AM
-- Server version: 10.4.10-MariaDB
-- PHP Version: 7.3.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `bankdb`
--

-- --------------------------------------------------------

--
-- Table structure for table `admin`
--

DROP TABLE IF EXISTS `admin`;
CREATE TABLE IF NOT EXISTS `admin` (
  `Admin_ID` varchar(20) COLLATE utf8mb4_bin NOT NULL,
  `Admin_Pass` int(6) NOT NULL,
  PRIMARY KEY (`Admin_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `admin`
--

INSERT INTO `admin` (`Admin_ID`, `Admin_Pass`) VALUES
('S001', 130613);

-- --------------------------------------------------------

--
-- Table structure for table `customer`
--

DROP TABLE IF EXISTS `customer`;
CREATE TABLE IF NOT EXISTS `customer` (
  `CusIC` varchar(15) COLLATE utf8mb4_bin NOT NULL,
  `CusName` varchar(100) COLLATE utf8mb4_bin NOT NULL,
  `CusAccountNo` varchar(20) COLLATE utf8mb4_bin NOT NULL,
  `CusAddress` varchar(250) COLLATE utf8mb4_bin NOT NULL,
  `CusPIN` varchar(6) COLLATE utf8mb4_bin NOT NULL,
  `CusCurrentAmount` decimal(10,2) NOT NULL,
  `CusStatus` varchar(10) COLLATE utf8mb4_bin NOT NULL DEFAULT 'Active',
  PRIMARY KEY (`CusIC`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `customer`
--

INSERT INTO `customer` (`CusIC`, `CusName`, `CusAccountNo`, `CusAddress`, `CusPIN`, `CusCurrentAmount`, `CusStatus`) VALUES
('011213040246', 'Sofia Ulya Binti Mohd Nasir', '723498411920', '27, Taman Sri Duyong 2', '131213', '1295.00', 'Active'),
('011021021234', 'Nabila Najwa', '173174122863', 'Taman Seremban 1', '102101', '508.00', 'Active'),
('010509040224', 'Nur Ezza Jeslin Chua', '607986607622', 'No 19, Taman Cheng Permain, 73648, Melaka', '112233', '2549.10', 'Active'),
('011230040096', 'Asfar Sarahah Binti Saparuddin', '464278248403', 'Kampungku Sayang, Melaka', '301201', '209.00', 'Active'),
('650208045342', 'Zuraini Binti Sainan', '874971597742', 'Taman Sri Duyong 2', '112233', '50.00', 'Deactive'),
('010701070102', 'Noor Solihah', '307502607824', 'Kampung Bawah, Gerik, 73455, Perak', '110701', '176.00', 'Active'),
('010327015678', 'Ainul Arifah Abd Rahim', '300072286643', 'Jalan Surau Batu, Taman Paya Dalam, Melaka', '123456', '100.00', 'Deactive'),
('000511017822', 'Tan Yu Nee', '494523872087', '5-2, Taman Skudai, 72844, Johor', '110501', '180.90', 'Active'),
('921109028944', 'Nur Ezzany Batrisyia', '1009567113066', '34, Ampang Point, 76879, Selangor', '567890', '150.00', 'Active');

-- --------------------------------------------------------

--
-- Table structure for table `customer_transaction`
--

DROP TABLE IF EXISTS `customer_transaction`;
CREATE TABLE IF NOT EXISTS `customer_transaction` (
  `CusTransID` varchar(15) COLLATE utf8mb4_bin NOT NULL,
  `TransDate` datetime NOT NULL,
  `TransAmount` decimal(10,2) NOT NULL,
  `TransDesc` varchar(50) COLLATE utf8mb4_bin DEFAULT NULL,
  `TransAccount` varchar(20) COLLATE utf8mb4_bin DEFAULT NULL,
  `CusIC` varchar(15) COLLATE utf8mb4_bin NOT NULL,
  `TransID` int(11) NOT NULL,
  PRIMARY KEY (`CusTransID`),
  KEY `FK_Customer` (`CusIC`),
  KEY `FK_Trans` (`TransID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `customer_transaction`
--

INSERT INTO `customer_transaction` (`CusTransID`, `TransDate`, `TransAmount`, `TransDesc`, `TransAccount`, `CusIC`, `TransID`) VALUES
('SB59748510123', '2022-01-27 01:52:47', '50.00', 'Savings Jan', '', '010509040224', 1),
('SB2527312977', '2022-03-27 15:48:58', '50.00', 'Savings', NULL, '011213040246', 1),
('SB65810105486', '2022-02-26 12:35:23', '20.00', 'Savings Feb', '', '011230040096', 1),
('SB42810727861', '2022-03-19 00:52:16', '50.00', 'hutang mcd', '607986607622', '011230040096', 2),
('SB07710103456', '2022-04-29 09:33:20', '500.00', NULL, NULL, '011213040246', 3),
('SB7237503376', '2022-04-29 09:34:33', '50.00', 'hutang grab', '607986607622', '011213040246', 2),
('SB471000101010', '2022-04-29 09:45:29', '30.00', 'Duit belanja', NULL, '011230040096', 1),
('SB9901809023', '2022-04-29 09:48:02', '50.00', 'duit ', '', '011230040096', 1),
('SB57240377108', '2022-05-21 15:05:28', '100.00', NULL, NULL, '011213040246', 3),
('SB8653208942', '2022-05-21 15:07:03', '100.00', 'Duit makan', '607986607622', '011213040246', 2),
('SB1148665140', '2022-05-12 12:08:26', '25.00', 'tudung', '723498411920', '010509040224', 2),
('SB1261343172', '2022-06-16 14:17:10', '50.00', '-', NULL, '011230040096', 1),
('SB83543554698', '2022-01-01 22:46:00', '21.50', 'AnW', '173174122863', '010701070102', 2),
('SB5711957258', '2022-06-20 21:12:11', '2500.00', '-', NULL, '010509040224', 1),
('SB4225360338', '2022-06-20 21:14:13', '200.00', NULL, NULL, '010509040224', 3),
('SB2834950029', '2022-06-20 21:21:55', '25.90', '4fingers', '494523872087', '010509040224', 2),
('SB5091354974', '2022-05-21 12:27:14', '5.00', 'grab', '173174122863', '000511017822', 2),
('SB0577487331', '2022-03-30 13:32:23', '50.00', NULL, NULL, '011230040096', 3),
('SB9724008219', '2022-06-21 13:52:04', '18.00', 'mcd', NULL, '011021021234', 1),
('SB8716053571', '2022-06-21 13:52:25', '5.00', NULL, NULL, '011021021234', 3),
('SB1110437097', '2022-06-21 13:53:58', '10.00', 'duit makan', '494523872087', '011021021234', 2),
('SB8743868698', '2022-06-21 15:23:16', '135.00', '-', NULL, '011230040096', 1),
('SB4446449168', '2022-06-21 15:24:13', '50.00', NULL, NULL, '011230040096', 3),
('SB6815313460', '2022-06-21 15:26:16', '26.00', 'cardigan', '307502607824', '011230040096', 2);

-- --------------------------------------------------------

--
-- Table structure for table `restore_trans`
--

DROP TABLE IF EXISTS `restore_trans`;
CREATE TABLE IF NOT EXISTS `restore_trans` (
  `CusTransID` varchar(15) COLLATE utf8mb4_bin NOT NULL,
  `TransDate` datetime NOT NULL,
  `TransAmount` decimal(10,2) NOT NULL,
  `TransDesc` varchar(50) COLLATE utf8mb4_bin DEFAULT NULL,
  `TransAccount` varchar(20) COLLATE utf8mb4_bin DEFAULT NULL,
  `CusIC` varchar(15) COLLATE utf8mb4_bin NOT NULL,
  `TransID` int(11) NOT NULL,
  `DeleteDate` datetime NOT NULL,
  PRIMARY KEY (`CusTransID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `restore_trans`
--

INSERT INTO `restore_trans` (`CusTransID`, `TransDate`, `TransAmount`, `TransDesc`, `TransAccount`, `CusIC`, `TransID`, `DeleteDate`) VALUES
('SB101106258366', '2022-05-28 21:40:09', '30.00', '', '', '011213040246', 3, '2022-06-13 01:26:18');

-- --------------------------------------------------------

--
-- Table structure for table `transaction`
--

DROP TABLE IF EXISTS `transaction`;
CREATE TABLE IF NOT EXISTS `transaction` (
  `TransID` int(11) NOT NULL AUTO_INCREMENT,
  `TransType` varchar(25) COLLATE utf8mb4_bin NOT NULL,
  PRIMARY KEY (`TransID`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `transaction`
--

INSERT INTO `transaction` (`TransID`, `TransType`) VALUES
(1, 'Deposit'),
(2, 'Transfer'),
(3, 'Withdrawal');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
