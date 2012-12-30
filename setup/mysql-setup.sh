#!/bin/bash

while getopts distchj: opt
do
  case "$opt" in
        j)
            JAVA7="$OPTARG"
            echo "Java7 path: $JAVA7" 
            
            ;;
        d)
            echo "Download" 
            wget http://cdn.mysql.com/Downloads/MySQL-5.5/mysql-5.5.28-linux2.6-x86_64.tar.gz
            wget http://cdn.mysql.com/Downloads/Connector-J/mysql-connector-java-5.1.22.tar.gz
            ;;
        i)
            echo "Install"
            tar -xvzf mysql-5.5.28-linux2.6-x86_64.tar.gz
            tar -xvzf mysql-connector-java-5.1.22.tar.gz
            mv mysql-5.5.28-linux2.6-x86_64 mysql
            export RQ_TMP_CLASSPATH=$CLASSPATH
            export CLASSPATH=$CLASSPATH:$PWD/mysql-connector-java-5.1.22/mysql-connector-java-5.1.22.jar
            sudo apt-get install libaio1
            sudo mkdir -p /var/lib/mysql_rq
            sudo chown -R mysql.mysql /var/lib/mysql_rq
            sudo mkdir -p /var/log/mysql_rq
            sudo chown -R mysql.mysql /var/log/mysql_rq
            
            sudo ./mysql/scripts/mysql_install_db --user=mysql --datadir=/var/lib/mysql_rq --basedir=./mysql
            sudo cp ./mysql/bin/mysqld ./mysql/bin/mysqld_rq
            sudo cp my.cnf /var/log/mysql_rq/my.cnf
            ;;
        s)
            echo "Start"
            cd mysql
            sudo ./bin/mysqld_safe --defaults-file=/var/log/mysql_rq/my.cnf --user=mysql --datadir=/var/lib/mysql_rq/ --port=3377 --mysqld=mysqld_rq  --log-error=/var/log/mysql_rq/log.err &
            cd ..
            ;;
        t)
            echo "Stop"
            cd mysql
            sudo ./bin/mysqladmin -S /var/lib/mysql_rq/mysql.sock shutdown &
            cd ..
            ;;
        c)
            echo "Clean everything"
            rm -fR ./mysql
            rm -fR ./mysql-connector-java-5.1.22
            sudo -fR /var/lib/mysql_rq
            sudo -fR /var/log/mysql_rq
            export CLASSPATH=$RQ_TMP_CLASSPATH
            unset RQ_TMP_CLASSPATH
            ;;            
        h)
            echo "usage: sudo $0 [-j java7/path] [-distcvh] "
            echo "       -j  Java7 path"
            echo "       -d  Download MySQL DB"
            echo "       -i  Install"
            echo "       -s  Start service"
            echo "       -t  Stop service"
            echo "       -c  Clean everything"
            echo "       -h  Help"
            exit 2
            ;;
        *)
            if [ "$OPTERR" != 1 ] || [ "${optspec:0:1}" = ":" ]; then
                echo "Non-option argument: '-${OPTARG}'"
            fi

    esac
done
