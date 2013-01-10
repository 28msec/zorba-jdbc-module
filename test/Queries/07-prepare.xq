import module namespace jdbc = "http://www.zorba-xquery.com/modules/jdbc";

variable $connection := jdbc:connect({
  "url": "jdbc:mysql://localhost:3307/?generateSimpleParameterMetadata=true",
  "user" : "root",
  "password" : ""});
  
jdbc:execute-update($connection, "USE `School`;");

variable $prep-stat := jdbc:prepare-statement($connection, "
SELECT A.idStudent, A.Name, Address, Birthday, Weight, Score 
FROM students A 
INNER JOIN scores B ON (A.idStudent = B.idStudent)
WHERE idCourse=?
");

jdbc:set-numeric($prep-stat,1,1);

variable $data-set := jdbc:execute-prepared($prep-stat);

variable $rows := jdbc:affected-rows($data-set);

if (jdbc:is-connected($connection))
 then { $rows }
 else false()
