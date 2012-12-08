import module namespace jdbc = "http://www.zorba-xquery.com/modules/jdbc";

variable $connection := jdbc:connect({
  "url": "jdbc:mysql://localhost/",
  "user" : "root",
  "password" : ""});
  
jdbc:execute-update($connection, "USE `School`;");

variable $data := jdbc:connection-options($connection);

if (jdbc:is-connected($connection))
 then { jdbc:disconnect($connection); $data }
 else false()
