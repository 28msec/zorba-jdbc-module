import module namespace jdbc = "http://www.zorba-xquery.com/modules/jdbc";

variable $connection := jdbc:connect({
  "url": "jdbc:mysql://localhost/",
  "user" : "root",
  "password" : ""});
  
jdbc:execute-update($connection, "DROP DATABASE `School`;");

if (jdbc:is-connected($connection))
 then { jdbc:disconnect($connection); true() }
 else false()
