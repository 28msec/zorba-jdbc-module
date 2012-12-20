import module namespace jdbc = "http://www.zorba-xquery.com/modules/jdbc";

variable $connection := jdbc:connect({
  "url": "jdbc:mysql://localhost/",
  "user" : "root",
  "password" : ""});

jdbc:execute-update($connection, "CREATE DATABASE /*!32312 IF NOT EXISTS*/`School` /*!40100 DEFAULT CHARACTER SET latin1 */;");
  
jdbc:execute-update($connection, "USE `School`;");

(: Table structures :)

jdbc:execute-update($connection, "
DROP TABLE IF EXISTS `courses`;
");
jdbc:execute-update($connection, "
CREATE TABLE `courses` (
  `idCourse` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `Course` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`idCourse`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
");
jdbc:execute-update($connection, "
INSERT INTO `courses`(`idCourse`,`Course`) VALUES (1,'Introduction to DBs'),(2,'Parallel Programming'),(3,'OpenSource XQuery Processors'),(4,'NoSQL Databases');
");

jdbc:execute-update($connection, "
DROP TABLE IF EXISTS `scores`;
");
jdbc:execute-update($connection, "
CREATE TABLE `scores` (
  `idCourse` bigint(20) DEFAULT NULL,
  `idStudent` bigint(20) DEFAULT NULL,
  `Score` float DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
");
jdbc:execute-update($connection, "
INSERT INTO `scores`(`idCourse`,`idStudent`,`Score`) VALUES (1,1,90),(1,2,90),(1,3,90),(2,2,90),(2,2,90),(2,3,90);
");

jdbc:execute-update($connection, "
DROP TABLE IF EXISTS `students`;
");
jdbc:execute-update($connection, "
CREATE TABLE `students` (
  `idStudent` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `Name` varchar(100) DEFAULT NULL,
  `Address` varchar(200) DEFAULT NULL,
  `Birthday` date DEFAULT NULL,
  `Weight` float DEFAULT NULL,
  PRIMARY KEY (`idStudent`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
");
jdbc:execute-update($connection, "
INSERT  INTO `students`(`idStudent`,`Name`,`Address`,`Birthday`,`Weight`) VALUES (1,'Rodolfo Ochoa','  10 Van De Graaff Dr # 1  Burlington, MA 01803','2012-12-06',160),(2,'Dana Florescu','10 Van De Graaff Dr # 1  Burlington, MA 01803','2012-12-13',150),(3,'Cezar Andrei','  10 Van De Graaff Dr # 1  Burlington, MA 01803','2012-12-27',160);
");



if (jdbc:is-connected($connection))
 then { jdbc:disconnect($connection); true() }
 else false()
