import module namespace jdbc = "http://www.zorba-xquery.com/modules/jdbc";

variable $connection := jdbc:connect({
  "url": "jdbc:mysql://localhost/",
  "user" : "root",
  "password" : ""});
  
jdbc:execute-update($connection, "USE `School`;");

variable $prep-stat := jdbc:prepare-statement($connection, "
SELECT A.idStudent, A.Name, Address, Birthday, Weight, Score 
FROM students A 
INNER JOIN scores B ON (A.idStudent = B.idStudent)
WHERE idCourse=?
");
jdbc:set-numeric($prep-stat, 1, 1.0);

variable $data-set := jdbc:execute-prepared($prep-stat);

variable $data := jdbc:result-set($data-set);

if (jdbc:is-connected($connection))
 then { jdbc:disconnect($connection); $data }
 else false()
