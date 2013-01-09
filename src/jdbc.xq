xquery version "3.0";

(:
 : Copyright 2006-2012 The FLWOR Foundation.
 :
 : Licensed under the Apache License, Version 2.0 (the "License");
 : you may not use this file except in compliance with the License.
 : You may obtain a copy of the License at
 :
 : http://www.apache.org/licenses/LICENSE-2.0
 :
 : Unless required by applicable law or agreed to in writing, software
 : distributed under the License is distributed on an "AS IS" BASIS,
 : WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 : See the License for the specific language governing permissions and
 : limitations under the License.
 :)

(:~
 : This module contains functions to connect to any JDBC datasource 
 : using jvm-util module to handle Java interaction.
 :
 : @author Rodolfo Ochoa
 : @project other data sources/jdbc module
 :)

module namespace jdbc = "http://www.zorba-xquery.com/modules/jdbc";

declare namespace err = "http://www.w3.org/2005/xqt-errors";
declare namespace ver = "http://www.zorba-xquery.com/options/versioning";
declare namespace an = "http://www.zorba-xquery.com/annotations";
declare option ver:module-version "1.0";

declare variable $jdbc:NOT-SUPPORTED    := "NOT-SUPPORTED";
declare variable $jdbc:READ-COMMITTED   := "READ-COMMITTED";
declare variable $jdbc:READ-UNCOMMITTED := "READ-UNCOMMITTED";
declare variable $jdbc:REPEATABLE-READ  := "REPEATABLE-READ";
declare variable $jdbc:SERIALIZABLE     := "SERIALIZABLE";

(:
 : 2 CONNECTION HANDLING
 :)

(:~
 : 2.1 Opens a connection to a database.
 : Returns a URI identifying the connection that has been opened. The implementing code determines from the $connection-config either explicitly (interpreting the driver attribute) or implicitly (using the type attribute) which driver it has to load.
 :
 : @param $connection-config json object that has the host and user informations.
 : @option "url" URL of the server, this option must be specified and should be declared according to JDBC specification.
 : @option "user" username for the server, this is optional.
 : @option "password" password for the server, this is optional.
 :
 : @error SQL28000 Authentication failed.
 : @error SQL08001 Connection error.
 : @error SQL40003 Isolation level not supported.
 : @error SQL001 Descriptive error, see attached message.
 :
 : @return Return an identifier that represents the connection to the server.
 :
 : Connection coonfiguration example:
 : { "url" : "jdbc:mysql://localhost/", 
 :   "user" : "root",
 :   "password" : "" }
 : 
 :)
declare %an:sequential function jdbc:connect(
                                     $connection-config as object() ) as xs:anyURI external;

(:~
 : 2.1 Opens a connection to a database with specified options.
 : Returns a URI identifying the connection that has been opened. The implementing code determines from the $connection-config either explicitly (interpreting the driver attribute) or implicitly (using the type attribute) which driver it has to load.
 :
 : @param $connection-config json object that has the host and user informations.
 : @option "url" URL of the server, this option must be specified and should be declared according to JDBC specification.
 : @option "user" username for the server, this option is optional.
 : @option "password" password for the server, this option is optional.
 : @param $options json object that specifies the connection options.
 : @option "autocommit" The created connection will have autocommit turned on if the value the attribute is set to "true".
 : @option "readonly" The created connection will be readonly if the value of the attribute is set to "true".
 : @option "isolation-level" The created connection will have the specified transaction isolation level, 
 : the following string typed in-scope variables represent the different transaction isolation levels 
 : that this attribute can be set to:
 : - $jsql:READ-COMMITTED
 : - $jsql:READ-UNCOMMITTED
 : - $jsql:REPEATABLE-READ
 : - $jsql:SERIALIZABLE
 : if no isolation level is provided by the user the connection will be created with the default 
 : isolation level of the database.
 :
 :
 : @error SQL28000 Authentication failed.
 : @error SQL08001 Connection error.
 : @error SQL40003 Isolation level not supported.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return Return an identifier that represents the connection to the server.
 :
 : Connection options example:
 : { "autocommit" : false, 
 :   "readonly"? : true,
 :   "isolation-level"? : $jdbc:READ-COMMITTED }
 : 
 :)
declare %an:sequential function jdbc:connect(
                                     $connection-config as object(), 
                                     $options as object()?) as xs:anyURI external;

(:~
 : 2.2 Verify if a connection is still active.
 :
 : @param $connection-id The identifier to the connection to be verify.
 :
 : @error SQL08003 Connection doesn't exist
 : @error SQL001 Descriptive error, see error in attached message
 :
 : @return Returns true if connected.
 :)
declare function jdbc:is-connected(
                                     $connection-id as xs:anyURI) as xs:boolean external;

(:~
 : 2.3 Returns a set with options for a specified connection.
 :
 : @param $connection-id The identifier to the connection to be verify.
 :
 : @error SQL08003 Connection doesn't exist
 : @error SQL08000 Connection is closed
 : @error SQL001 Descriptive error, see error in attached message
 :
 : @return Returns and object with the connection options.
 : The returned options are equal to the options specified in function jdbc:connect. 
 : Consequently, the options are specified as follows:
 :    { "autocommit" : xs:boolean,
 :      "readonly" : xs:boolean,
 :      "isolation-level" : xs:string }
 :
 :)
declare function jdbc:connection-options(
                                      $connection-id as xs:anyURI) as object() external;

(:
 : 3 TRANSACTIONS
 :)

(:~
 : 3.1 Commit current transaction from an active connection.
 :
 : @param $connection-id The identifier to the connection to be commited.
 : 
 : @error SQL08003 Connection doesn't exist
 : @error SQL08000 Connection is closed
 : @error SQL001 Descriptive error, see error in attached message
 :
 : @return This function returns an empty-sequence()
 :)
declare %an:sequential function jdbc:commit(
                                      $connection-id as xs:anyURI) as empty-sequence() external;

(:~
 :  3.2 Rollback the current transaction of a connection.
 :
 : @param $connection-id The identifier to the connection to be rollbacked.
 :
 : @error SQL08003 Connection doesn't exist
 : @error SQL08000 Connection is closed
 : @error SQL001 Descriptive error, see error in attached message
 :
 : @return This function returns an empty-sequence()
 :)
declare %an:sequential function jdbc:rollback(
                                      $connection-id as xs:anyURI) as empty-sequence() external;


(:
 : 4 SIMPLE STATEMENTS
 :)

(:~
 : 4.1 Executes any kind of SQL statement. 
 :
 : @param $connection-id The identifier to an active connection.
 : @param $sql The query string to be executed.
 : 
 : @error SQL08003 Connection doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return Return an identifier that represents a DataSet.
 :)
declare %an:sequential function jdbc:execute( 
                                      $connection-id as xs:anyURI,
                                      $sql as xs:string ) as xs:anyURI external;

(:~
 : 4.2 Executes non-updating SQL statements.
 :
 : @param $connection-id The identifier to an active connection.
 : @param $sql The query string to be executed.
 : 
 : @error SQL08003 Connection doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL005 The statement is Updating type.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return Return an object with the result data rows from the query provided,
 :  the data rows are defined as follows:
 :   { column:value* }*
 :  Every row is represented by an object of column-value representation of the returned SQL result.
 :
 :)
declare function jdbc:execute-query( 
                                      $connection-id as xs:anyURI, 
                                      $sql as xs:string) as object()* external;

(:~
 : 4.3 Executes updating SQL statements.
 :
 : @param $connection-id The identifier to an active connection.
 : @param $sql The query string to be executed.
 : 
 : @error SQL08003 Connection doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL005 The statement is Read-only type.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return Returns an xs:integer with the number of affected rows.
 :)
declare function jdbc:execute-update(
                                      $connection-id as xs:anyURI,
                                      $sql as xs:string) as xs:integer external;


(:
 :  5 PREPARED STATEMENTS
 :)

(:~
 :  5.1 Creates a prepared statement for multiple executions with diferent values.
 :
 : @param $connection-id The identifier to an active connection.
 : @param $sql The query string to be executed.
 : 
 : @error SQL08003 Connection doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return Return an identifier that represents the prepared statement.
 :
 : Example:
 : jsql:prepare-statement($connection, "SELECT * FROM users WHERE id=? AND age>?")
 :)
declare %an:sequential function jdbc:prepare-statement(
                                      $connection-id as xs:anyURI,
                                      $sql as xs:string) as xs:anyURI external;

(:~
 : 5.2.1 Set the value of the designated parameter with the given value, this function will assign only numeric values.
 :
 : @param $prepared-statement The identifier to a prepared statement.
 : @param $parameter-index The index from the parameter to be set.
 : @param $value The value to be set.
 : 
 : @error SQL003 Prepared statement doesn't exist.
 : @error SQL007 Parameter casting error.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return This function returns an empty-sequence()
 :)
declare %an:sequential function jdbc:set-numeric(
                                      $prepared-statement as xs:anyURI, 
                                      $parameter-index as xs:decimal, 
                                      $value as xs:anyAtomicType) as empty-sequence() external;

(:~
 : 5.2.2 Set the value of the designated parameter with the given value, this function will assign only string values.
 :
 : @param $prepared-statement The identifier to a prepared statement.
 : @param $parameter-index The index from the parameter to be set.
 : @param $value The value to be set.
 : 
 : @error SQL003 Prepared statement doesn't exist.
 : @error SQL007 Parameter casting error.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return This function returns an empty-sequence()
 :)
declare %an:sequential function jdbc:set-string(
                                      $prepared-statement as xs:anyURI,
                                      $parameter-index as xs:integer,
                                      $value as xs:string) as empty-sequence() external;

(:~
 : 5.2.3 Set the value of the designated parameter with the given value, this function will assign only boolean values.
 :
 : @param $prepared-statement The identifier to a prepared statement.
 : @param $parameter-index The index from the parameter to be set.
 : @param $value The value to be set.
 : 
 : @error SQL003 Prepared statement doesn't exist.
 : @error SQL007 Parameter casting error.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return This function returns an empty-sequence()
 :)
declare %an:sequential function jdbc:set-boolean(
                                      $prepared-statement as xs:anyURI, 
                                      $parameter-index as xs:integer,
                                      $value as xs:boolean) as empty-sequence() external;

(:~
 : 5.2.4 Set the value of the designated parameter with the given value, this function will assign only null values if possible.
 :
 : @param $prepared-statement The identifier to a prepared statement.
 : @param $parameter-index The index from the parameter to be set.
 : 
 : @error SQL003 Prepared statement doesn't exist.
 : @error SQL007 Parameter casting error.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return This function returns an empty-sequence()
 :)
declare %an:sequential function jdbc:set-null(
                                      $prepared-statement as xs:anyURI,
                                      $parameter-index as xs:integer) as empty-sequence() external;

(:~
 : 5.2.5 Set the value of the designated parameter with the given value, 
 : this function will assign any value you send 
 : and it will try to cast to the correct type.
 :
 : @param $prepared-statement The identifier to a prepared statement.
 : @param $parameter-index The index from the parameter to be set.
 : @param $value The value to be set.
 : 
 : @error SQL003 Prepared statement doesn't exist.
 : @error SQL007 Parameter casting error.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return This function returns an empty-sequence()
 :)
declare %an:sequential function jdbc:set-value(
                                      $prepared-statement as xs:anyURI, 
                                      $parameter-index as xs:decimal, 
                                      $value as xs:anyAtomicType) as empty-sequence() external;

(:~
 : 5.3 Clears the current parameter values immediately.
 :
 : @param $prepared-statement The identifier to a prepared statement.
 : 
 : @error SQL003 Prepared statement doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return This function returns an empty-sequence()
 :)
declare %an:sequential function jdbc:clear-params(
                                      $prepared-statement as xs:anyURI) as empty-sequence() external;

(:~
 : 5.4 Retrieves the number, types and properties of the prepared statement parameters.
 :
 : @param $prepared-statement The identifier to a prepared statement.
 : 
 : @error SQL003 Prepared statement doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return This function returns the parameters metadata associated with a prepared statement. 
 : In other words, it returns information about the column name associated with the parameter, the type, etc.
 : The metadata node returned by this function is defined as follows:
 :   {
 :     columns: [{
 :       "name": xs:string,
 :       "type": xs:string
 :       }]
 :   }
 : @option "name" The name of the column.
 : @option "type" The SQL type of the column.
 :)
declare function jdbc:parameter-metadata(
                                      $prepared-statement as xs:anyURI) as object() external;

(:~
 : 5.5 Executes SQL statements prepared with 5.1 jsql:prepare-statement with values set
 : and returns and identifier to a Dataset.
 :
 : @param $prepared-statement The identifier to a prepared statement.
 : 
 : @error SQL003 Prepared statement doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :)
declare %an:sequential function jdbc:execute-prepared(
                                      $prepared-statement as xs:anyURI) as xs:anyURI external;

(:~
 : 5.6 Executes a non-updating SQL statement prepared with 5.1 jsql:prepare-statement.
 :
 : @param $prepared-statement The identifier to a prepared statement.
 : 
 : @error SQL003 Prepared statement doesn't exist.
 : @error SQL005 The prepared statement is an updating query.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return Return an object with the result data rows from the query processed with the parameter values provided,
 :  the data rows are defined as follows:
 :   { column:value* }*
 :  Every row is represented by an object of column-value representation of the returned SQL result.
 :)
declare function jdbc:execute-query-prepared(
                                      $prepared-statement as xs:anyURI) as object()* external;

(:~
 : 5.7 Executes an updating SQL statement prepared with 5.1 jsql:prepare-statement.
 :
 : @param $prepared-statement The identifier to a prepared statement.
 : 
 : @error SQL003 Prepared statement doesn't exist.
 : @error SQL006 The prepared statement is a non-updating query.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return Returns an xs:integer with the number of affected rows.
 :)
declare function jdbc:execute-update-prepared(
                                      $prepared-statement as xs:anyURI) as xs:integer external;

(:~
 : 5.8 Closes and frees from memory any prepared SQL statement created with jdbc:prepare-statement
 :
 : @param $prepared-statement The identifier to a prepared statement.
 : 
 : @error SQL003 Prepared statement doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return This function returns an empty-sequence()
 :)
declare %an:sequential function jdbc:close-prepared(
                                      $prepared-statement as xs:anyURI) as empty-sequence() external;


(:
 :  6 DATASETS
 :)

(:~
 : 6.1 This function returns a sequence of objects representing the rows of data from a non-updating query.
 :
 : @param $dataset-id The identifier to a DataSet.
 : 
 : @error SQL008 DataSet doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return Return an object with the result data rows from the DataSet provided,
 :  the data rows are defined as follows:
 :   { column:value* }*
 :  Every row is represented by an object of column-value representation of the returned SQL result.
 :)
declare function jdbc:result-set(
                                      $dataset-id as xs:anyURI) as object()* external;

(:~
 : 6.2 Return the metadata of the result of a particular DataSet.
 :
 : @param $dataset-id The identifier to a DataSet.
 : 
 : @error SQL008 DataSet doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return This function returns the metadata associated with an executed DataSet. More in detail, it returns information about column names, types, and whether a column can contain a null value.
 : The metadata information can only be returned for DataSets that have been executed explicitly using the jsql:execute function.
 : 
 : The metadata node returned by this function is defined as follows:
 : {
 :   "columns": [ {
 :       "name": xs:string,
 :       "type": xs:string,
 :       "autoincrement"? = xs:boolean,
 :       "nillable"? = xs:boolean } * ]
 : }
 : @option "name" The name of the column.
 : @option "type" The SQL type of the column.
 : @option "autoincrement" is true if this column is automatically maintained.
 : @option "nillable" If the colums can contain NULL values this attribute will be set to true.
 :
 : If the query is an updating query, then the result object will return the number of affected rows like:
 : { "affectedrows": xs:integer }
 :)
declare function jdbc:metadata(
                                      $dataset-id as xs:anyURI) as object() external;

(:~
 : 6.3 Return the number of affected rows of a particular DataSet.
 :
 : @param $dataset-id The identifier to a DataSet.
 : 
 : @error SQL008 DataSet doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return Returns an xs:integer with the number of affected rows.
 :)
declare function jdbc:affected-rows(
                                      $dataset-id as xs:anyURI) as xs:integer external;

(:~
 : 6.4 Closes and free resources from a particular DataSet.
 :
 : @param $dataset-id The identifier to a DataSet.
 : 
 : @error SQL008 DataSet doesn't exist.
 : @error SQL08000 Connection is closed.
 : @error SQL001 Descriptive error, see error in attached message.
 :
 : @return This function returns an empty-sequence()
 :)
declare %an:sequential function jdbc:close-dataset(
                                      $dataset-id as xs:anyURI) as empty-sequence() external;
