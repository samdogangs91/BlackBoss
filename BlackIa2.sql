-- MySQL dump 10.15  Distrib 10.0.23-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: BlackIa2
-- ------------------------------------------------------
-- Server version	10.0.23-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `adjectif`
--

DROP TABLE IF EXISTS `adjectif`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `adjectif` (
  `adj_id` int(11) NOT NULL,
  `singAdj` int(11) DEFAULT NULL,
  `femAdj` int(11) DEFAULT NULL,
  `plurAdj` int(11) DEFAULT NULL,
  `plurFemAdj` int(11) DEFAULT NULL,
  `isMasc` tinyint(1) DEFAULT NULL,
  `isFem` tinyint(1) DEFAULT NULL,
  `isSing` tinyint(1) DEFAULT NULL,
  `isPlur` tinyint(1) DEFAULT NULL,
  `canMasc` tinyint(1) DEFAULT NULL,
  `canFem` tinyint(1) DEFAULT NULL,
  `canSing` tinyint(1) DEFAULT NULL,
  `isDenm` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`adj_id`),
  KEY `fk_singAdj` (`singAdj`),
  KEY `fk_femAdj` (`femAdj`),
  KEY `fk_plurAdj` (`plurAdj`),
  KEY `fk_plurFemAdj` (`plurFemAdj`),
  CONSTRAINT `fk_adj_id` FOREIGN KEY (`adj_id`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_femAdj` FOREIGN KEY (`femAdj`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_plurAdj` FOREIGN KEY (`plurAdj`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_plurFemAdj` FOREIGN KEY (`plurFemAdj`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_singAdj` FOREIGN KEY (`singAdj`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `adjectif`
--

LOCK TABLES `adjectif` WRITE;
/*!40000 ALTER TABLE `adjectif` DISABLE KEYS */;
/*!40000 ALTER TABLE `adjectif` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `conjugaison`
--

DROP TABLE IF EXISTS `conjugaison`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `conjugaison` (
  `conj_id` int(11) NOT NULL,
  `verb_id` int(11) DEFAULT NULL,
  `je` int(11) DEFAULT NULL,
  `tu` int(11) DEFAULT NULL,
  `il` int(11) DEFAULT NULL,
  `nous` int(11) DEFAULT NULL,
  `vous` int(11) DEFAULT NULL,
  `ils` int(11) DEFAULT NULL,
  `tps` int(10) unsigned DEFAULT NULL,
  `isEtat` tinyint(1) DEFAULT NULL,
  `isAux` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`conj_id`),
  KEY `fk_verb_id` (`verb_id`),
  KEY `fk_je` (`je`),
  KEY `fk_tu` (`tu`),
  KEY `fk_il` (`il`),
  KEY `fk_nous` (`nous`),
  KEY `fk_vous` (`vous`),
  KEY `fk_ils` (`ils`),
  KEY `fk_tps` (`tps`),
  CONSTRAINT `fk_il` FOREIGN KEY (`il`) REFERENCES `verbe` (`verbe_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_ils` FOREIGN KEY (`ils`) REFERENCES `verbe` (`verbe_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_je` FOREIGN KEY (`je`) REFERENCES `verbe` (`verbe_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_nous` FOREIGN KEY (`nous`) REFERENCES `verbe` (`verbe_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_tps` FOREIGN KEY (`tps`) REFERENCES `string` (`string_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_tu` FOREIGN KEY (`tu`) REFERENCES `verbe` (`verbe_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_verb_id` FOREIGN KEY (`verb_id`) REFERENCES `verbe` (`verbe_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_vous` FOREIGN KEY (`vous`) REFERENCES `verbe` (`verbe_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `conjugaison`
--

LOCK TABLES `conjugaison` WRITE;
/*!40000 ALTER TABLE `conjugaison` DISABLE KEYS */;
/*!40000 ALTER TABLE `conjugaison` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `determinant`
--

DROP TABLE IF EXISTS `determinant`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `determinant` (
  `det_id` int(11) NOT NULL,
  `singDet` int(11) DEFAULT NULL,
  `femDet` int(11) DEFAULT NULL,
  `plurDet` int(11) DEFAULT NULL,
  `plurFemDet` int(11) DEFAULT NULL,
  `isMasc` tinyint(1) DEFAULT NULL,
  `isFem` tinyint(1) DEFAULT NULL,
  `isSing` tinyint(1) DEFAULT NULL,
  `isPlur` tinyint(1) DEFAULT NULL,
  `canMasc` tinyint(1) DEFAULT NULL,
  `canFem` tinyint(1) DEFAULT NULL,
  `canSing` tinyint(1) DEFAULT NULL,
  `isDenm` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`det_id`),
  KEY `fk_singDet` (`singDet`),
  KEY `fk_femDet` (`femDet`),
  KEY `fk_plurDet` (`plurDet`),
  KEY `fk_plurFemDet` (`plurFemDet`),
  CONSTRAINT `fk_det_id` FOREIGN KEY (`det_id`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_femDet` FOREIGN KEY (`femDet`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_plurDet` FOREIGN KEY (`plurDet`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_plurFemDet` FOREIGN KEY (`plurFemDet`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_singDet` FOREIGN KEY (`singDet`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `determinant`
--

LOCK TABLES `determinant` WRITE;
/*!40000 ALTER TABLE `determinant` DISABLE KEYS */;
/*!40000 ALTER TABLE `determinant` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `instruction`
--

DROP TABLE IF EXISTS `instruction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `instruction` (
  `ins_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` int(10) unsigned DEFAULT NULL,
  `argT` int(10) unsigned DEFAULT NULL,
  `retourT` int(10) unsigned DEFAULT NULL,
  `cont` int(10) unsigned DEFAULT NULL,
  `prior` int(11) DEFAULT NULL,
  `assoc` int(10) unsigned DEFAULT NULL,
  `isOp` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`ins_id`),
  KEY `fk_name` (`name`),
  KEY `fk_argT` (`argT`),
  KEY `fk_retourT` (`retourT`),
  KEY `fk_cont` (`cont`),
  KEY `fk_assoc` (`assoc`),
  CONSTRAINT `fk_argT` FOREIGN KEY (`argT`) REFERENCES `string` (`string_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_assoc` FOREIGN KEY (`assoc`) REFERENCES `string` (`string_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_cont` FOREIGN KEY (`cont`) REFERENCES `string` (`string_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_name` FOREIGN KEY (`name`) REFERENCES `string` (`string_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_retourT` FOREIGN KEY (`retourT`) REFERENCES `string` (`string_id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `instruction`
--

LOCK TABLES `instruction` WRITE;
/*!40000 ALTER TABLE `instruction` DISABLE KEYS */;
INSERT INTO `instruction` VALUES (1,1,2,3,4,15,5,1),(5,6,3,3,3,15,5,1),(6,7,3,3,3,2,5,1),(8,8,3,3,3,2,9,1),(9,10,3,3,3,9,9,1),(10,11,3,3,3,9,9,1),(11,12,3,3,3,15,5,1),(12,13,3,3,3,15,5,1),(13,14,3,3,3,15,5,1),(14,15,3,3,3,8,9,1),(15,16,3,3,3,8,9,1),(16,17,3,3,3,2,9,1),(17,18,3,3,3,2,9,1),(18,19,3,3,3,13,9,1),(19,20,3,3,3,14,9,1),(20,21,3,3,3,7,9,1),(21,22,3,3,3,7,9,1),(22,23,3,3,3,2,9,1),(23,24,3,3,3,3,5,1),(24,25,3,3,3,6,9,1),(25,26,3,3,3,6,9,1),(26,27,3,3,3,5,9,1),(27,28,3,3,3,5,9,1),(28,29,3,3,3,5,9,1),(29,30,3,3,3,8,9,1),(30,31,3,3,3,8,9,1);
/*!40000 ALTER TABLE `instruction` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `list_char`
--

DROP TABLE IF EXISTS `list_char`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `list_char` (
  `list_char_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `cont` varchar(26) DEFAULT NULL,
  `size` int(11) DEFAULT NULL,
  PRIMARY KEY (`list_char_id`)
) ENGINE=InnoDB AUTO_INCREMENT=57 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `list_char`
--

LOCK TABLES `list_char` WRITE;
/*!40000 ALTER TABLE `list_char` DISABLE KEYS */;
INSERT INTO `list_char` VALUES (1,'string::=',9),(2,'string;string;',14),(3,'',0),(4,'If(arg.size()==2 && retour',111),(5,'gauche',6),(6,'=',1),(7,'++',2),(8,'--',2),(9,'droite',6),(10,'==',2),(11,'!=',2),(12,'+=',2),(13,'*=',2),(14,'/=',2),(15,'>=',2),(16,'<=',2),(17,'->',2),(18,'[]',2),(19,'&&',2),(20,'||',2),(21,'<<',2),(22,'>>',2),(23,'.',1),(24,'!',1),(25,'+',1),(26,'-',1),(27,'*',1),(28,'/',1),(29,'%',1),(30,'>',1),(31,'<',1),(44,'x:int;y:int;',12),(45,'int;',4),(47,'int;intint;',11),(53,'int;int;',8),(55,'Return x+y;',11),(56,'Return ?[0]+?[1];',17);
/*!40000 ALTER TABLE `list_char` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mot`
--

DROP TABLE IF EXISTS `mot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mot` (
  `mot_id` int(11) NOT NULL,
  `basicName` int(10) unsigned DEFAULT NULL,
  `type` int(10) unsigned DEFAULT NULL,
  `syn` int(10) unsigned DEFAULT NULL,
  `conn` int(10) unsigned DEFAULT NULL,
  `isDenm` tinyint(1) DEFAULT NULL,
  `fun` int(10) unsigned DEFAULT NULL,
  `def` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`mot_id`),
  KEY `fk_basicName` (`basicName`),
  KEY `fk_type` (`type`),
  KEY `fk_syn` (`syn`),
  KEY `fk_conn` (`conn`),
  KEY `fk_fun` (`fun`),
  KEY `fk_def` (`def`),
  CONSTRAINT `fk_basicName` FOREIGN KEY (`basicName`) REFERENCES `string` (`string_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_conn` FOREIGN KEY (`conn`) REFERENCES `string` (`string_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_def` FOREIGN KEY (`def`) REFERENCES `string` (`string_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_fun` FOREIGN KEY (`fun`) REFERENCES `string` (`string_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_syn` FOREIGN KEY (`syn`) REFERENCES `string` (`string_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_type` FOREIGN KEY (`type`) REFERENCES `string` (`string_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mot`
--

LOCK TABLES `mot` WRITE;
/*!40000 ALTER TABLE `mot` DISABLE KEYS */;
/*!40000 ALTER TABLE `mot` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `nom`
--

DROP TABLE IF EXISTS `nom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `nom` (
  `nom_id` int(11) NOT NULL,
  `sing` int(11) DEFAULT NULL,
  `fem` int(11) DEFAULT NULL,
  `plur` int(11) DEFAULT NULL,
  `plurFem` int(11) DEFAULT NULL,
  `isMasc` tinyint(1) DEFAULT NULL,
  `isFem` tinyint(1) DEFAULT NULL,
  `isSing` tinyint(1) DEFAULT NULL,
  `isPlur` tinyint(1) DEFAULT NULL,
  `canMasc` tinyint(1) DEFAULT NULL,
  `canFem` tinyint(1) DEFAULT NULL,
  `canSing` tinyint(1) DEFAULT NULL,
  `isDenm` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`nom_id`),
  KEY `fk_sing` (`sing`),
  KEY `fk_fem` (`fem`),
  KEY `fk_plur` (`plur`),
  KEY `fk_plurFem` (`plurFem`),
  CONSTRAINT `fk_fem` FOREIGN KEY (`fem`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_nom_id` FOREIGN KEY (`nom_id`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_plur` FOREIGN KEY (`plur`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_plurFem` FOREIGN KEY (`plurFem`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_sing` FOREIGN KEY (`sing`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `nom`
--

LOCK TABLES `nom` WRITE;
/*!40000 ALTER TABLE `nom` DISABLE KEYS */;
/*!40000 ALTER TABLE `nom` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prenom`
--

DROP TABLE IF EXISTS `prenom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `prenom` (
  `pre_id` int(11) NOT NULL,
  `isMasc` tinyint(1) DEFAULT NULL,
  `isFem` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`pre_id`),
  CONSTRAINT `fk_pre_id` FOREIGN KEY (`pre_id`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `prenom`
--

LOCK TABLES `prenom` WRITE;
/*!40000 ALTER TABLE `prenom` DISABLE KEYS */;
/*!40000 ALTER TABLE `prenom` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `string`
--

DROP TABLE IF EXISTS `string`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `string` (
  `string_id` int(10) unsigned NOT NULL,
  `size` int(11) DEFAULT NULL,
  PRIMARY KEY (`string_id`),
  CONSTRAINT `fk_string_id` FOREIGN KEY (`string_id`) REFERENCES `list_char` (`list_char_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `string`
--

LOCK TABLES `string` WRITE;
/*!40000 ALTER TABLE `string` DISABLE KEYS */;
INSERT INTO `string` VALUES (1,9),(2,14),(3,0),(4,111),(5,6),(6,1),(7,2),(8,2),(9,6),(10,2),(11,2),(12,2),(13,2),(14,2),(15,2),(16,2),(17,2),(18,2),(19,2),(20,2),(21,2),(22,2),(23,1),(24,1),(25,1),(26,1),(27,1),(28,1),(29,1),(30,1),(31,1),(44,12),(45,4),(47,11),(53,8),(55,11),(56,17);
/*!40000 ALTER TABLE `string` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `type`
--

DROP TABLE IF EXISTS `type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `type` (
  `name` varchar(200) NOT NULL,
  `cont` varchar(200) DEFAULT NULL,
  `descr` varchar(200) DEFAULT NULL,
  `meth` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `type`
--

LOCK TABLES `type` WRITE;
/*!40000 ALTER TABLE `type` DISABLE KEYS */;
INSERT INTO `type` VALUES ('adjectif','adj_id:mot;singAdj:mot;femAdj:mot;plurAdj:mot;plurFemAdj:mot;isMasc:bool;isFem:bool;isSing:bool;isPlur:bool;canMasc:bool;canFem:bool;canSing:bool;isDenm:bool;','',''),('conjugaison','conj_id:int;verb_id:verbe;je:verbe;tu:verbe;il:verbe;nous:verbe;vous:verbe;ils:verbe;tps:string;isEtat:bool;isAux:bool;','',''),('determinant','det_id:mot;singDet:mot;femDet:mot;plurDet:mot;plurFemDet:mot;isMasc:bool;isFem:bool;isSing:bool;isPlur:bool;canMasc:bool;canFem:bool;canSing:bool;isDenm:bool;','',''),('instruction','ins_id:int;name:string;argT:string;retourT:string;cont:string;prior:int;assoc:string;isOp:bool;','',''),('list_char','list_char_id:int;size:int;','',''),('mot','mot_id:int;basicName:string;type:string;syn:string;conn:string;isDenm:bool;fun:string;def:string;','',''),('nom','nom_id:mot;sing:mot;fem:mot;plur:mot;plurFem:mot;isMasc:bool;isFem:bool;isSing:bool;isPlur:bool;canMasc:bool;canFem:bool;canSing:bool;isDenm:bool;','',''),('prenom','pre_id:mot;isMasc:bool;isFem:bool;','',''),('string','string_id:list_char;size:int;','',''),('verbe','verbe_id:mot;inf:mot;partPast:mot;partPre:mot;numPers:int;temps:string;isEtat:bool;isAux:bool;numGr:int;','','');
/*!40000 ALTER TABLE `type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `verbe`
--

DROP TABLE IF EXISTS `verbe`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `verbe` (
  `verbe_id` int(11) NOT NULL,
  `inf` int(11) DEFAULT NULL,
  `partPast` int(11) DEFAULT NULL,
  `partPre` int(11) DEFAULT NULL,
  `numPers` int(11) DEFAULT NULL,
  `temps` int(10) unsigned DEFAULT NULL,
  `isEtat` tinyint(1) DEFAULT NULL,
  `isAux` tinyint(1) DEFAULT NULL,
  `numGr` int(11) DEFAULT NULL,
  PRIMARY KEY (`verbe_id`),
  KEY `fk_inf` (`inf`),
  KEY `fk_partPast` (`partPast`),
  KEY `fk_partPre` (`partPre`),
  KEY `fk_temps` (`temps`),
  CONSTRAINT `fk_inf` FOREIGN KEY (`inf`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_partPast` FOREIGN KEY (`partPast`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_partPre` FOREIGN KEY (`partPre`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_temps` FOREIGN KEY (`temps`) REFERENCES `string` (`string_id`) ON DELETE CASCADE,
  CONSTRAINT `fk_verbe_id` FOREIGN KEY (`verbe_id`) REFERENCES `mot` (`mot_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `verbe`
--

LOCK TABLES `verbe` WRITE;
/*!40000 ALTER TABLE `verbe` DISABLE KEYS */;
/*!40000 ALTER TABLE `verbe` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-09-11  0:13:20
