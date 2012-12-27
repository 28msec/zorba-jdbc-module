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
 :  2 CONNECTION HANDLING
 :)

(:
 : Opens a connection to a relational database.
 : Returns a URI identifying the connection that has been opened. The implementing code determines from the $connection-config either explicitly (interpreting the driver attribute) or implicitly (using the type attribute) which driver it has to load.
 :
 : @param $connection-config
 : In detail, the $connection-config is specified as follows:
 : { "url" : xs:string, 
 :   "user"? : xs:string,
 :   "password"? : xs:string }
 : 
 : @error 
 :)
declare %an:sequential function jdbc:connect(
                                     $connection-config as object() ) as xs:anyURI external;
(:
 :  Opens a connection to a relational database with specified options.
 :)
declare %an:sequential function jdbc:connect(
                                     $connection-config as object(), 
                                     $options as object()?) as xs:anyURI external;
(:
 :  2.2 Closes an open database connection.
 :)
declare %an:sequential function jdbc:disconnect(
                                     $connection-id as xs:anyURI) external;
(:
 :  2.3 Returns true if a connection is connected.
 :)
declare function jdbc:is-connected(
                                     $connection-id as xs:anyURI) as xs:boolean external;
(:
 :  2.4 Returns the options set for a connection.
 :)
declare function jdbc:connection-options(
                                     $connection-id as xs:anyURI) as object() external;


(:
 :  3 TRANSACTIONS
 :)

(:
 :  3.1 Commit current transaction of a connection.
 :)
declare %an:sequential function jdbc:commit(
                                     $connection-id as xs:anyURI) as xs:anyURI external;
(:
 :  3.2 Rollback the current transaction of a connection.
 :)
declare %an:sequential function jdbc:rollback(
                                     $connection-id as xs:anyURI) as xs:anyURI external;


(:
 :  4 SIMPLE STATEMENTS
 :)

(:
 :  4.1 Executes any kind of SQL statement provided as $sql string. 
 :)
declare %an:sequential function jdbc:execute( 
                                     $connection-id as xs:anyURI,
                                     $sql as xs:string ) as xs:anyURI external;
(:
 :  4.2 Executes any read-only SQL statement provided as $sql string.
 :)
declare function jdbc:execute-query( 
                      $connection-id as xs:anyURI, 
                      $sql as xs:string) as object()* external;
(:
 :  4.3 Executes deterministic read-only SQL statements provided as $sql string.
 :)
declare function jdbc:execute-query-deterministic( 
                      $connection-id as xs:anyURI,
                      $sql as xs:string) as object()* external;
(:
 :  4.4 Executes only updating SQL statements provided as $sql string.
 :)
declare function jdbc:execute-update(
                      $connection-id as xs:anyURI,
                      $sql as xs:string) as xs:integer external;


(:
 :  5 PREPARED STATEMENTS
 :)

(:
 :  5.1 Creates a prepared statement for multiple executions.
 :)
declare %an:sequential function jdbc:prepare-statement(
                                     $connection-id as xs:anyURI,
                                     $sql as xs:string) as xs:anyURI external;
(:
 :  5.2.1 Set the value of the designated parameter with the given value.
 :)
declare %an:sequential function jdbc:set-numeric(
                                     $prepared-statement as xs:anyURI, 
                                     $parameter-index as xs:decimal, 
                                     $value as xs:anyAtomicType) as empty-sequence() external;
(:
 :  5.2.2 Set the value of the designated parameter with the given value.
 :)
declare %an:sequential function jdbc:set-string(
                                     $prepared-statement as xs:anyURI,
                                     $parameter-index as xs:integer,
                                     $value as xs:string) as empty-sequence() external;
(:
 :  5.2.3 Set the value of the designated parameter with the given value.
 :)
declare %an:sequential function jdbc:set-boolean(
                                     $prepared-statement as xs:anyURI, 
                                     $parameter-index as xs:integer,
                                     $value as xs:boolean) as empty-sequence() external;
(:
 :  5.2.4 Set the value of the designated parameter with the given value.
 :)
declare %an:sequential function jdbc:set-null(
                                     $prepared-statement as xs:anyURI,
                                     $parameter-index as xs:integer) as empty-sequence() external;
(:
 :  5.3 Clears the current parameter values immediately.
 :)
declare %an:sequential function jdbc:clear-params(
                                     $prepared-statement as xs:anyURI) external;
(:
 :  5.4 Retrieves the number, types and properties of the prepared statement parameters.
 :)
declare function jdbc:parameter-metadata(
                      $prepared-statement as xs:anyURI) as object() external;
(:
 :  5.5 Executes SQL statements prepared with 5.1 jsql:prepare-statement.
 :)
declare %an:sequential function jdbc:execute-prepared(
                                     $prepared-statement as xs:anyURI) as xs:anyURI external;
(:
 :  5.6 Executes a non-updating SQL statement prepared with 5.1 jsql:prepare-statement.
 :)
declare function jdbc:execute-query-prepared(
                      $prepared-statement as xs:anyURI) as object()* external;
(:
 :  5.7 Executes an updating SQL statement prepared with 5.1 jsql:prepare-statement.
 :)
declare function jdbc:execute-update-prepared(
                      $prepared-statement as xs:anyURI) as xs:integer external;


(:
 :  6 DATASETS
 :)

(:
 :  6.1 This function returns a sequence of objects representing the rows of data from a non-updating query.
 :)
declare function jdbc:result-set(
                      $dataset-id as xs:anyURI) as object()* external;
(:
 :  6.2 Return the metadata of the result of a particular DataSet.
 :)
declare function jdbc:metadata(
                      $dataset-id as xs:anyURI) as object() external;
(:
 :  6.3 Return the number of affected rows of a particular DataSet.
 :)
declare function jdbc:affected-rows(
                      $dataset-id as xs:anyURI) as xs:integer external;

