#!/bin/bash

# Default location to dump stuff and create databases
WORKDIR=~/jdbc-module-mysql

# MySQL'd "datadir"
DATADIR=${WORKDIR}/datadir

# Location for the logs
LOGDIR=${WORKDIR}/log

while getopts distchw: opt
do
  case "$opt" in
        w)
            WORKDIR="$OPTARG"
            echo "Data directory: $WORKDIR"
            ;;
        d)
            echo "Download"
            mkdir -p $WORKDIR
            cd $WORKDIR
            wget http://cdn.mysql.com/Downloads/MySQL-5.5/mysql-5.5.28-linux2.6-x86_64.tar.gz
            wget http://cdn.mysql.com/Downloads/Connector-J/mysql-connector-java-5.1.22.tar.gz
            ;;
        i)
            echo "Install"
            (
            cd $WORKDIR
            tar -xvzf mysql-5.5.28-linux2.6-x86_64.tar.gz
            tar -xvzf mysql-connector-java-5.1.22.tar.gz
            mv mysql-5.5.28-linux2.6-x86_64 mysql
            sudo apt-get install libaio1
            mkdir -p $DATADIR $LOGDIR $DATADIR/innodb_log
            ./mysql/scripts/mysql_install_db --user=$USER --datadir=$DATADIR --basedir=./mysql --no-defaults
            )
            MODDIR=`dirname $0`
            cp $MODDIR/my.cnf $DATADIR/my.cnf
            ;;
        s)
            echo "Start"
            cd $WORKDIR/mysql
            ./bin/mysqld_safe --defaults-file=$DATADIR/my.cnf --user=$USER --pid-file=$DATADIR/mysql_rq.pid --datadir=$DATADIR --log-error=$LOGDIR/log.err &
            cd ..
            ;;
        t)
            echo "Stop"
            kill `cat $DATADIR/mysql_rq.pid`   
            ;;
        c)
            echo "Clean everything"
            cd $WORKDIR
            rm -fR ./mysql
            rm -fR ./mysql-connector-java-5.1.22
            ;;            
        h)
            echo "usage: $0 [-distcvh] [-w path]"
            echo "       -d  Download MySQL DB"
            echo "       -i  Install"
            echo "       -s  Start service"
            echo "       -t  Stop service"
            echo "       -c  Clean everything"
            echo "       -w path  Path to store everything (default ~/jdbc-module-mysql)"
            echo "       -h  Help"
            exit 2
            ;;
        *)
            if [ "$OPTERR" != 1 ] || [ "${optspec:0:1}" = ":" ]; then
                echo "Non-option argument: '-${OPTARG}'"
            fi

    esac
done
