import module namespace jdbc = "http://www.zorba-xquery.com/modules/jdbc";

variable $connection := jdbc:connect({
  "url": "jdbc:mysql://localhost:3307/",
  "user" : "root",
  "password" : ""});
  
jdbc:execute-update($connection, "USE `School`;");

variable $data := jdbc:execute-query($connection, "
SELECT A.idStudent, A.Name, Address, Birthday, Weight, Score 
FROM students A 
INNER JOIN scores B ON (A.idStudent = B.idStudent)
WHERE idCourse=1
");

if (jdbc:is-connected($connection))
 then { $data }
 else false()
