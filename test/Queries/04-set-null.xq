import module namespace jdbc = "http://www.zorba-xquery.com/modules/jdbc";

variable $connection := jdbc:connect({
  "url": "jdbc:mysql://localhost:3077/?generateSimpleParameterMetadata=true",
  "user" : "root",
  "password" : ""});
  
jdbc:execute-update($connection, "USE `School`;");

variable $prep-stat := jdbc:prepare-statement($connection, "
SELECT A.idStudent, A.Name, Address, Birthday, Weight, Score 
FROM students A 
INNER JOIN scores B ON (A.idStudent = B.idStudent)
WHERE idCourse=?
");

jdbc:set-null($prep-stat,1);

variable $metadata := jdbc:parameter-metadata($prep-stat);

if (jdbc:is-connected($connection))
 then { $metadata }
 else false()
